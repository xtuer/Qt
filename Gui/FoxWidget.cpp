#include "FoxWidget.h"
#include <QDebug>
#include <QPainter>

FoxWidget::FoxWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground);
    qDebug() << "FoxWidget()";
}

FoxWidget::~FoxWidget() {
    qDebug() << "~FoxWidget()";
}

void FoxWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(QColor(255, 255, 0, 100));
    painter.drawRoundedRect(0, 0, width() - 0, height() - 0, 10, 10);
}
