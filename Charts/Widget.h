#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class SelectableChartView;

namespace Ui {
class Widget;
}

namespace QtCharts {
class QChart;
class QChartView;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void createChart();
    void handleEvents();

    Ui::Widget *ui;
    QtCharts::QChart *chart;
    SelectableChartView *chartView;
};

#endif // WIDGET_H
