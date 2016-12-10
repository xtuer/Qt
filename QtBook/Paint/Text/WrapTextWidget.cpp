#include "WrapTextWidget.h"
#include "ui_WrapTextWidget.h"
#include <QPainter>

WrapTextWidget::WrapTextWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WrapTextWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

WrapTextWidget::~WrapTextWidget() {
    delete ui;
}

void WrapTextWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString text = "QPainter provides highly optimized functions to do "
                   "most of the drawing GUI programs require. It can draw "
                   "everything from simple lines to complex shapes "
                   "like pies and chords. "
                   "看看是否也支持中文呢，如果不支持那就悲剧了！";

    QRect rect(20, 35, 200, 80);
    int flags = Qt::TextWordWrap | Qt::AlignRight; // 自动换行，向右对齐

    painter.drawText(20, 20, text);
    painter.drawRect(rect); // 画出矩形，可以看到超出此矩形的部分文本不可见
    painter.drawText(rect, flags, text);
}
