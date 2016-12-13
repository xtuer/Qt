#include <QApplication>
#include <QWidget>
#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QHBoxLayout>

using namespace QtCharts;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 创建平滑曲线上点的序列
    QSplineSeries *splineSeries = new QSplineSeries();
    splineSeries->setName("spline");

    splineSeries->append(0, 6);
    splineSeries->append(2, 4);
    splineSeries->append(3, 8);
    splineSeries->append(7, 4);
    splineSeries->append(10, 5);
    *splineSeries << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    // 创建散列点的序列
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);
    scatterSeries->append(0, 6);
    scatterSeries->append(2, 4);
    scatterSeries->append(3, 8);
    scatterSeries->append(7, 4);
    scatterSeries->append(10, 5);
    *scatterSeries << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    // 使用点的序列创建图标
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->setTitle("平滑曲线");
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, 10);

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
