#include "AutoResizeTextWidget.h"
#include "ui_AutoResizeTextWidget.h"
#include <QPainter>

AutoResizeTextWidget::AutoResizeTextWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AutoResizeTextWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

AutoResizeTextWidget::~AutoResizeTextWidget() {
    delete ui;
}

void AutoResizeTextWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString text = "QPainter provides highly optimized functions to do "
                   "most of the drawing GUI programs require. It can draw "
                   "everything from simple lines to complex shapes "
                   "like pies and chords. "
                   "看看是否也支持中文呢，如果不支持那就悲剧了！";

    int width = this->width() - 40; // 显示文本的宽度，为窗口的宽度减去 40 像素
    int flags = Qt::TextWordWrap; // 自动换行

    // 计算文本在指定宽度下的包围矩形
    QFontMetrics metrics = painter.fontMetrics();
    QRect textBoundingRect = metrics.boundingRect(QRect(0, 0, width, 0), flags, text);
    painter.translate(20, 20);
    painter.drawRect(textBoundingRect);
    painter.drawText(textBoundingRect, flags, text);
}
