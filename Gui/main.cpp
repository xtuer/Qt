#include <QApplication>
#include <QChart>
#include <QSplineSeries>
#include <QChartView>

using namespace QtCharts;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSplineSeries *splineSeries = new QSplineSeries();
    splineSeries->setPointsVisible();

    for (int x = 0; x < 100000; x += 10) {
        splineSeries->append(x, qrand() % 100);
    }

    QChart *chart = new QChart();
    chart->addSeries(splineSeries);
    chart->createDefaultAxes();
    chart->axisX()->setRange(1200, 1300);

    QChartView *chartView = new QChartView(chart);
    chartView->resize(400, 300);
    chartView->show();

    return app.exec();
}
