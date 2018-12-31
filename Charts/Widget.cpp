#include "Widget.h"
#include "ui_Widget.h"
#include "SelectableChartView.h"

#include <QSplineSeries>
#include <QChartView>
#include <QMouseEvent>
#include <QDateTime>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    createChart();  // 创建图标
    handleEvents(); // 事件处理
}

Widget::~Widget() {
    delete ui;
}

void Widget::createChart() {
    // [1] 创建曲线上点的序列
    QSplineSeries *splineSeries = new QSplineSeries();
    QLineSeries   *lineSeries   = new QLineSeries();
    splineSeries->setName("Spline");
    lineSeries->setName("Line");

    qint64 current = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for (int i = 0; i < 100; ++i, current += 600000) {
        // splineSeries->append(QPoint(i, qrand() % 100));
        // lineSeries->append(QPoint(i, qrand() % 100 - 30));
        splineSeries->append(current, qrand() % 100);
        lineSeries->append(current, qrand() % 100 - 30);
    }

    // [2] 使用点的序列创建图表
    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(lineSeries);
    // chart->createDefaultAxes(); // 默认没有创建坐标轴

    // [3] 创建坐标轴
    QDateTimeAxis *xAxis = new QDateTimeAxis();
    xAxis->setFormat("yyyy-MM-dd HH:mm:ss");
    xAxis->setTitleText("时间");
    xAxis->setLabelsAngle(-60);
    xAxis->setTickCount(8);

    QValueAxis *yAxis = new QValueAxis();

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);

    // [4] 曲线使用坐标轴
    splineSeries->attachAxis(xAxis);
    splineSeries->attachAxis(yAxis);
    lineSeries->attachAxis(xAxis);
    lineSeries->attachAxis(yAxis);

    // TODO: 使用前先设置坐标轴的 objectName
    xAxis->setObjectName("时间");
    yAxis->setObjectName("温度");
    // axisY->hide();

    // [5] chartView 声明和定义都要使用 SelectableChartView
    chartView = new SelectableChartView(chart);
    layout()->replaceWidget(ui->widget, chartView);

    // 设置灭菌的相关参数
    chartView->setSterilizationParams(QDateTime::currentDateTime().addSecs(5000),
                                      QDateTime::currentDateTime().addSecs(50000),
                                      85);
    // 设置显示灭菌辅助线，默认是隐藏的
    chartView->setSterilizationMarkersVisible(true);
}

void Widget::handleEvents() {
    connect(ui->timeRangeButton, &QPushButton::clicked, [this] {
        chartView->getDateTimeAxis()->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addDays(10));
    });

    connect(ui->minTempButton, &QPushButton::clicked, [this] {
        qDebug() << chartView->getMinTemperature();
    });
}
