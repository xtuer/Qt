#include "PolylineWidget.h"
#include "ui_PolylineWidget.h"
#include <QPainter>

PolylineWidget::PolylineWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PolylineWidget) {
    ui->setupUi(this);
}

PolylineWidget::~PolylineWidget() {
    delete ui;
}

void PolylineWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(20, 20);

    static const QPointF points[4] = {
        QPointF(0.0, 100.0),
        QPointF(20.0, 0.0),
        QPointF(100.0, 0.0),
        QPointF(120.0, 100.0)
    };

    painter.drawPolyline(points, 4);
}
