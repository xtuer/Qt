#include "MySeries.h"
#include <QDebug>

MySeries::MySeries() {

}

QList<QPointF> MySeries::points() const {
    QList<QPointF> points;
    points << QPointF(0, 10) << QPointF(20, 20) << QPointF(30, 0) << QPointF(50, 10);

    qDebug() << "points()";

    return points;
}
