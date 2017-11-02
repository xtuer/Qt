#include <QApplication>
#include <QWidget>
#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QHBoxLayout>
#include "MySeries.h"
#include <QDebug>

using namespace QtCharts;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 创建平滑曲线上点的序列
    QSplineSeries *splineSeries = new MySeries();
    splineSeries->setUseOpenGL(true);

    splineSeries->setName("spline");
    splineSeries->append(0, 6);
    splineSeries->append(2, 4);
    splineSeries->append(3, 8);
    splineSeries->append(7, 4);
    splineSeries->append(10, 5);
    *splineSeries << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    // 使用点的序列创建图标
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(splineSeries);
    chart->setTitle("平滑曲线");
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, 10);
    chart->axisX()->setRange(-10, 60);

    // 显示图标的 view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Widget 相关
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);

    QWidget w;
    w.setWindowTitle("QChart 实现的平滑曲线");
    w.resize(600, 300);
    w.setLayout(layout);
    w.show();

    return a.exec();
}
