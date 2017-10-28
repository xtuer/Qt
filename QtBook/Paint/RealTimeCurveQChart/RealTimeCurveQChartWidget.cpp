#include "RealTimeCurveQChartWidget.h"
#include <QDateTime>
#include <QHBoxLayout>

RealTimeCurveQChartWidget::RealTimeCurveQChartWidget(QWidget *parent) : QWidget(parent) {
    maxSize = 31; // 只存储最新的 31 个数据
    maxX = 300;
    maxY = 100;

    splineSeries = new QSplineSeries();
    scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);

    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->legend()->hide();
    chart->setTitle("实时动态曲线");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 300);
    chart->axisY()->setRange(0, maxY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);

    timerId = startTimer(200);
    qsrand(QDateTime::currentDateTime().toTime_t());
}

RealTimeCurveQChartWidget::~RealTimeCurveQChartWidget() {
}

void RealTimeCurveQChartWidget::timerEvent(QTimerEvent *event) {
    // 产生一个数据，模拟不停的接收到新数据
    if (event->timerId() == timerId) {
        int newData = qrand() % (maxY + 1);
        dataReceived(newData);
    }
}

void RealTimeCurveQChartWidget::dataReceived(int value) {
    data << value;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        splineSeries->clear();
        scatterSeries->clear();
        int dx = maxX / (maxSize-1);
        int less = maxSize - data.size();

        for (int i = 0; i < data.size(); ++i) {
            splineSeries->append(less*dx+i*dx, data.at(i));
            scatterSeries->append(less*dx+i*dx, data.at(i));
        }
    }
}
