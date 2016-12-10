#include "DrawSimpleTextWidget.h"
#include "ui_DrawSimpleTextWidget.h"
#include <QPainter>

DrawSimpleTextWidget::DrawSimpleTextWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DrawSimpleTextWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

DrawSimpleTextWidget::~DrawSimpleTextWidget() {
    delete ui;
}

void DrawSimpleTextWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Times", 150, QFont::Bold));
    QFontMetrics metrics = painter.fontMetrics();
    int x = 0;
    int y = metrics.ascent();
    painter.drawText(x, y, "jEh");
}
