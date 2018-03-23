#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include <QChartView>
#include <QList>

using namespace QtCharts;

class QLabel;

class SelectableChartView : public QChartView {
public:
    SelectableChartView(QChart *chart);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *createSelectionLabel(qreal startX, qreal endX, qreal startY, qreal endY, QRect geometry);
    QPoint pressedPointAtChart;
    QList<QLabel*> selections; // 保存选区的 Label
};

#endif // CHARTVIEW_H
