#ifndef CALLOUTCHARTWIDGET_H
#define CALLOUTCHARTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>

class Callout;

namespace Ui {
class CalloutChartWidget;
}

namespace QtCharts {
class QChart;
class QSplineSeries;
}

class CalloutChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalloutChartWidget(QWidget *parent = nullptr);
    ~CalloutChartWidget() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void initialize();
    void handleEvents();

    Ui::CalloutChartWidget *ui;
    QtCharts::QChart *chart;
    QtCharts::QSplineSeries *splineSeries;
    QGraphicsView *view;
    Callout *callout;
};

#endif // CALLOUTCHARTWIDGET_H
