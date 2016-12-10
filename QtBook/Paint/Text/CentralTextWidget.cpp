#include "CentralTextWidget.h"
#include "ui_CentralTextWidget.h"
#include <QPainter>

CentralTextWidget::CentralTextWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CentralTextWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

CentralTextWidget::~CentralTextWidget() {
    delete ui;
}

void CentralTextWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Times", 150, QFont::Bold));

    QRect rect(20, 20, 300, 200);
    painter.drawRect(rect);

    // 居中绘制文本
    QFontMetrics metrics = painter.fontMetrics();
    int stringHeight = metrics.ascent() + metrics.descent(); // 不算 line gap
    int stringWidth = metrics.width("jEh"); // 字符串的宽度
    int x = rect.x() + (rect.width() - stringWidth) / 2;
    int y = rect.y() + (rect.height() - stringHeight) / 2 + metrics.ascent();
    painter.drawText(x, y, "jEh");

    // 绘制字符串的包围矩形
    y = rect.y() + (rect.height() - stringHeight) / 2;
    painter.setPen(Qt::lightGray);
    painter.drawRect(x, y, stringWidth, stringHeight);
}
