#include "CalloutChartWidget.h"
#include "ui_CalloutChartWidget.h"
#include "Callout.h"

#include <QChartView>
#include <QSplineSeries>
#include <QLineSeries>
#include <QChart>
#include <QDebug>
#include <QPen>

CalloutChartWidget::CalloutChartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CalloutChartWidget) {
    ui->setupUi(this);

    initialize();
    handleEvents();
}

CalloutChartWidget::~CalloutChartWidget() {
    delete ui;
}

void CalloutChartWidget::initialize() {
    // 创建平滑曲线上点的序列
    splineSeries = new QSplineSeries();
    splineSeries->setName("Spline");
//    splineSeries->setPen(QPen(Qt::blue, 3));

    splineSeries->append(0, 6);
    splineSeries->append(2, 4);
    splineSeries->append(3, 8);
    splineSeries->append(7, 4);
    splineSeries->append(10, 5);
    *splineSeries << QPointF(11, 1) << QPointF(13, 23) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    lineSeries = new QLineSeries();
    lineSeries->setName("Line");
    lineSeries->append(1, 7);
    lineSeries->append(3, 8);
    lineSeries->append(4, 5);
    lineSeries->append(8, 3);
    lineSeries->append(15, 23);
    lineSeries->append(18, 33);

    // 使用点的序列创建图表: 标题, 坐标轴, 图例都是属于图表的
    chart = new QChart();
    chart->addSeries(lineSeries);
    chart->addSeries(splineSeries);
    chart->legend()->hide();
    chart->setTitle("平滑曲线");
    chart->createDefaultAxes(); // 默认没有创建坐标轴
    chart->setMinimumSize(640, 480);
    chart->setAcceptHoverEvents(true);

    // 提示: QChart 继承了 QGraphicsItem，QChartView 继承了 QGraphicsView
    //      Callout 继承了 QGraphicsItem，可以作为 item 放到 chart 中
    newCallout();

    // 显示图标的 view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->layout()->replaceWidget(ui->placeholderWidget, chartView);
}

void CalloutChartWidget::resizeEvent(QResizeEvent *event) {
//    view->scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
//    chart->resize(event->size());

    QWidget::resizeEvent(event);
}

void CalloutChartWidget::handleEvents() {
    QList<QAbstractAxis *> yAxes = chart->axes(Qt::Vertical, splineSeries);

    // zoom 是变焦的意思，in 表示将镜头拉近，自然放大
    connect(ui->zoomInButton, &QPushButton::clicked, [=] {
        yAxes[0]->setRange(5, 10);
    });

    connect(ui->zoomOutButton, &QPushButton::clicked, [=] {
        yAxes[0]->setRange(0, 40);
    });

    // 访问所有 series 的上的 callout，每个 series 上的 callout 已经按照 x 坐标排好序了
    connect(ui->outputButton, &QPushButton::clicked, [this] {
        for (QXYSeries *s : seriesCallouts.keys()) {
            for (Callout *c : *seriesCallouts.value(s)) {
                qDebug() << c->series()->name() << ": " << c->x() << ", " << c->y();
            }
        }
    });

    handleCalloutEvents(lineSeries);
    handleCalloutEvents(splineSeries);
}

// 处理 series 的信号槽，鼠标移动到 series 上时显示 callout，点击 series 时保存 callout
void CalloutChartWidget::handleCalloutEvents(QXYSeries *series) {
    connect(series, &QXYSeries::hovered, [=] (const QPointF &point, bool state) {
        showCallout(series, point, state);
    });

    connect(series, &QXYSeries::clicked, [=](const QPointF &point) {
        showCallout(series, point, true);

        saveCallout();
    });
}

// 显示 callout
void CalloutChartWidget::showCallout(QXYSeries *series, const QPointF &point, bool state) {
    if (state) {
        callout->setText(QString("%1\nX: %2\nY: %3").arg(series->name()).arg(point.x()).arg(point.y()));
        callout->setZValue(1000);
        callout->setAnchor(point, series);
        callout->updateGeometry();
        callout->show();
    } else {
        callout->hide();
    }
}

// 创建新的 callout，双击 callout 则删除它
void CalloutChartWidget::newCallout() {
    callout = new Callout(chart);
    callout->hide();

    connect(callout, &Callout::doubleClicked, [=](Callout *c) {
        deleteCallout(c);
    });
}

// 保存 callout
void CalloutChartWidget::saveCallout() {
    QXYSeries *series = callout->series();

    if (!seriesCallouts.contains(series)) {
        seriesCallouts.insert(series, new QList<Callout *>());
    }

    QList<Callout *> *callouts = seriesCallouts.value(series);

    int pos = 0;
    for (int i = 0; i < callouts->size(); i++) {
        if (callouts->at(i)->x() < callout->x()) {
            pos++;
        }
    }

    // 保存当前 callout 到 seriesCallouts 中，然后创建一个新的 callout
    callouts->insert(pos, callout);
    newCallout();
}

// 删除 callout
void CalloutChartWidget::deleteCallout(Callout *callout) {
    QList<Callout *> *callouts = seriesCallouts.value(callout->series());

    if (nullptr != callouts) {
        if (callouts->removeOne(callout)) {
            delete callout;
        }
    }
}
