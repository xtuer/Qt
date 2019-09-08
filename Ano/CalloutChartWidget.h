#ifndef CALLOUTCHARTWIDGET_H
#define CALLOUTCHARTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QHash>

class Callout;

namespace Ui {
class CalloutChartWidget;
}

namespace QtCharts {
class QChart;
class QLineSeries;
class QSplineSeries;
class QXYSeries;
class QAbstractSeries;
}

using namespace QtCharts;

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
    void handleCalloutEvents(QXYSeries *series);

    void showCallout(QXYSeries *series, const QPointF &point, bool state); // 显示 callout
    void newCallout();                    // 创建 callout
    void saveCallout();                   // 保存 callout
    void deleteCallout(Callout *callout); // 删除 callout

    Ui::CalloutChartWidget *ui;
    QChart *chart;
    QLineSeries   *lineSeries;
    QSplineSeries *splineSeries;
    QGraphicsView *view;

    Callout *callout;
    QHash<QXYSeries*, QList<Callout*>*> seriesCallouts; // series 上的 callouts
};

#endif // CALLOUTCHARTWIDGET_H
