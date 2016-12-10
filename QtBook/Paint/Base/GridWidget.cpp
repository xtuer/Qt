#include "GridWidget.h"
#include "ui_GridWidget.h"
#include <QPainter>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GridWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

GridWidget::~GridWidget() {
    delete ui;
}

void GridWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.translate(30, 30);

    int w   = 300;
    int h   = 210;
    int gap = 30;

    // 画水平线
    for (int y = 0; y <= h; y += gap) {
        painter.drawLine(0, y, w, y);
    }

    // 画垂直线
    for (int x = 0; x <= w; x += gap) {
        painter.drawLine(x, 0, x, h);
    }
}
