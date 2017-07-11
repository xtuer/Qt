#include "DraggableLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

DraggableLabel::DraggableLabel(const QString &text, QWidget *parent) : QLabel(text, parent) {
    this->setAttribute(Qt::WA_StyledBackground, true);
    pressedPos = QPoint();
}

void DraggableLabel::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.drawText(rect(), Qt::AlignCenter, text());

    painter.setBrush(Qt::blue);
    painter.drawRect(resizeHandle());
}

void DraggableLabel::mousePressEvent(QMouseEvent *event) {
    event->accept();
    pressedPos = event->globalPos();
    posAsPressed = pos();
    sizeAsPressed = size();
    resizeMode = resizeHandle().contains(event->pos());
}

void DraggableLabel::mouseReleaseEvent(QMouseEvent *event) {
    event->accept();
    pressedPos = QPoint();
}

void DraggableLabel::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if (pressedPos.isNull()) { return; }

    QPoint d = event->globalPos() - pressedPos;

    if (resizeMode) {
        this->resize(sizeAsPressed.width() + d.x(), sizeAsPressed.height() + d.y());
    } else {
        this->move(posAsPressed + d);
    }
}

QRect DraggableLabel::resizeHandle() const {
    int s = 8;
    return QRect(width() - s, height() - s, s, s);
}
