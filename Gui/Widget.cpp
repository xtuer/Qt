#include "Widget.h"
#include "ui_Widget.h"
#include <QPainter>
#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>

using namespace QtCharts;

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    QSplineSeries *series = new QSplineSeries();
    series->setName("spline");

    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QScatterSeries *series2 = new QScatterSeries();
    series2->setMarkerSize(8);
    series2->append(0, 6);
    series2->append(2, 4);
    series2->append(3, 8);
    series2->append(7, 4);
    series2->append(10, 5);
    *series2 << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->addSeries(series2);
    chart->setTitle("Simple spline chart example");
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, 10);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout()->addWidget(chartView);

}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
