#include "RoundRectWidget.h"
#include "ui_RoundRectWidget.h"
#include <QPainter>

RoundRectWidget::RoundRectWidget(QWidget *parent) : QWidget(parent), ui(new Ui::RoundRectWidget) {
    ui->setupUi(this);
}

RoundRectWidget::~RoundRectWidget() {
    delete ui;
}

void RoundRectWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::lightGray);
    painter.translate(30, 30);

    painter.drawRoundRect(0, 0, 100, 100, 50, 50); // 50%, 50%
    painter.drawRoundedRect(130, 0, 100, 100, 50, 50, Qt::AbsoluteSize); // 50px, 50px
    painter.drawRoundedRect(260, 0, 100, 100, 100, 100, Qt::RelativeSize); // 100%, 100%
}
