#ifndef MYSERIES_H
#define MYSERIES_H
#include <QSplineSeries>

class MySeries : public QtCharts::QSplineSeries {
public:
    MySeries();

    QList<QPointF> points() const;
};

#endif // MYSERIES_H
