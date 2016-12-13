#include "RealTimeCurveQChartWidget.h"
#include <QDateTime>
#include <QHBoxLayout>

RealTimeCurveQChartWidget::RealTimeCurveQChartWidget(QWidget *parent) : QWidget(parent) {
    maxSize = 31;   // 只存储最新的 31 个数据
    maxValue = 100; // 数据的最大值为 100，因为我们生成的随机数为 [0, 100]
    timerId = startTimer(200);
    qsrand(QDateTime::currentDateTime().toTime_t());

    splineSeries = new QSplineSeries();
    scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);

    // 预先分配坐标，这样在 dataReceived 中直接替换坐标了
    for (int i = 0; i < maxSize; ++i) {
        splineSeries->append(i * 10, -10);
        scatterSeries->append(i * 10, -10);
    }

    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->legend()->hide();
    chart->setTitle("实时动态曲线");
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, maxValue);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

RealTimeCurveQChartWidget::~RealTimeCurveQChartWidget() {
}

void RealTimeCurveQChartWidget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        // 模拟不停的接收到新数据
        int newData = qrand() % (maxValue + 1);
        dataReceived(newData);
    }
}

void RealTimeCurveQChartWidget::dataReceived(int value) {
    data << value;

    // 数据个数超过了指定值，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    if (isVisible()) {
        // 界面被隐藏后就没有必要绘制数据的曲线了
        // 替换曲线中现有数据
        int delta = maxSize - data.size();
        for (int i = 0; i < data.size(); ++i) {
            splineSeries->replace(delta+i, splineSeries->at(delta+i).x(), data.at(i));
            scatterSeries->replace(delta+i, scatterSeries->at(delta+i).x(), data.at(i));
        }
    }
}
