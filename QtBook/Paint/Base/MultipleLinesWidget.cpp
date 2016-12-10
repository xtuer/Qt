#include "MultipleLinesWidget.h"
#include "ui_MultipleLinesWidget.h"
#include <QPainter>

MultipleLinesWidget::MultipleLinesWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MultipleLinesWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

MultipleLinesWidget::~MultipleLinesWidget() {
    delete ui;
}

void MultipleLinesWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(20, 20);

    static const QPointF points[4] = {
        QPointF(0.0, 100.0),
        QPointF(20.0, 0.0),
        QPointF(100.0, 0.0),
        QPointF(120.0, 100.0)
    };

    painter.drawLines(points, 2); // 4 个点连成 2 条线
}
