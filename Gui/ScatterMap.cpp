#include "ScatterMap.h"
#include "Scatter.h"
#include <QPainter>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>

ScatterMap::ScatterMap(QWidget *parent) : QWidget(parent) {
    QRandomGenerator random(QDateTime::currentDateTime().toMSecsSinceEpoch());

    for (int i = 0; i < 3; ++i) {
        Scatter *scatter = new Scatter(random.bounded(1.0), random.bounded(1.0), this);
        scatters.append(scatter);

        connect(scatter, &Scatter::aboutRemove, this, &ScatterMap::removeScatter);
    }
}

void ScatterMap::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    int w = width();
    int h = height();

    painter.setPen(QPen(Qt::gray, 0));
    painter.drawRect(0, 0, w-1, h-1);
}

void ScatterMap::resizeEvent(QResizeEvent *event) {
    for (Scatter *scatter : scatters) {
        scatter->updatePosition();
    }

    QWidget::resizeEvent(event);
}

void ScatterMap::removeScatter(Scatter *scatter) {
    scatters.removeOne(scatter);
    scatter->deleteLater();
}
