#include "TextPathWidget.h"
#include "ui_TextPathWidget.h"
#include <QPainter>

TextPathWidget::TextPathWidget(QWidget *parent) : QWidget(parent), ui(new Ui::TextPathWidget) {
    ui->setupUi(this);
}

TextPathWidget::~TextPathWidget() {
    delete ui;
}

void TextPathWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString text("jEh 太极");
    QFont font("Xingkai SC", 120, QFont::Bold);
    QFontMetrics metrics(font);

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = y1 + metrics.height();

    // 使用渐变填充
    QLinearGradient gradient(x1, y1 + 40, x2, y2);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(1, QColor(50, 50, 50));

    // 用文本创建 QPainterPath，第一、二个参数是 baseline 的坐标
    QPainterPath path;
    path.addText(x1, y1 + metrics.ascent(), font, text);

    painter.translate(20, -20);
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawPath(path);
}
