#include "PolygonWidget.h"
#include "ui_PolygonWidget.h"
#include <QPainter>

PolygonWidget::PolygonWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PolygonWidget) {
    ui->setupUi(this);
}

PolygonWidget::~PolygonWidget() {
    delete ui;
}

void PolygonWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(20, 20);

    static const QPointF points[4] = {
        QPointF(0.0, 100.0),
        QPointF(20.0, 0.0),
        QPointF(100.0, 0.0),
        QPointF(120.0, 100.0)
    };

    painter.setBrush(Qt::yellow);
    painter.drawPolygon(points, 4);
}
