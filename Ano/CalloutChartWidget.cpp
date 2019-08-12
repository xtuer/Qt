#include "CalloutChartWidget.h"
#include "ui_CalloutChartWidget.h"
#include "Callout.h"

#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QDebug>

using namespace QtCharts;

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

    splineSeries->append(0, 6);
    splineSeries->append(2, 4);
    splineSeries->append(3, 8);
    splineSeries->append(7, 4);
    splineSeries->append(10, 5);
    *splineSeries << QPointF(11, 1) << QPointF(13, 23) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    // 使用点的序列创建图表: 标题, 坐标轴, 图例都是属于图表的
    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->legend()->hide();
    chart->setTitle("平滑曲线");
    chart->createDefaultAxes(); // 默认没有创建坐标轴
    chart->setMinimumSize(640, 480);
    chart->setAcceptHoverEvents(true);

    // 提示: QChart 继承了 QGraphicsItem，QChartView 继承了 QGraphicsView
    //      Callout 继承了 QGraphicsItem，可以作为 item 放到 chart 中
    callout = new Callout(chart);
    callout->hide();

    // 显示图标的 view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->layout()->replaceWidget(ui->placeholderWidget, chartView);

//    view = new QGraphicsView(new QGraphicsScene);
//    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->scene()->addItem(chart);
//    this->layout()->replaceWidget(ui->placeholderWidget, view);
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

    connect(splineSeries, &QSplineSeries::hovered, [=] (const QPointF &point, bool state) {
        if (state) {
//            QPointF pos = chart->mapToPosition(point, splineSeries) - QPointF(-10, 15);
//            callout->setPos(pos);
            callout->setText(QString("X: %1\nY: %2").arg(point.x()).arg(point.y()));
            callout->setZValue(1000);
            callout->setAnchor(point);
            callout->updateGeometry();
            callout->show();
        } else {
            callout->hide();
        }
    });
}
