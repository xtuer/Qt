#ifndef CHART_H
#define CHART_H
#include <QChart>

class Chart : public QtCharts::QChart {
public:
    Chart(QGraphicsItem *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CHART_H
