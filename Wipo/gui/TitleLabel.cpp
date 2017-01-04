#include "TitleLabel.h"
#include <QPainter>

TitleLabel::TitleLabel(QWidget *parent) : QWidget(parent) {
    background.load(":/img/title.png");
}

void TitleLabel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), background);
}
