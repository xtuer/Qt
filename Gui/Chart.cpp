#include "Chart.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Chart::Chart(QGraphicsItem *parent) : QChart(parent) {

}

void Chart::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    qDebug() << event->pos();
//    qDebug() << plotArea();
    event->ignore();
}

void Chart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << event->type();
    event->ignore();
}
