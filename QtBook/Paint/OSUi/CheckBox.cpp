#include "CheckBox.h"
#include <QPainter>

CheckBox::CheckBox(const QString &text, bool checked, const QRect &boundingRect)
    : Widget(boundingRect), checked(checked), text(text) {
}

void CheckBox::paint(QPainter *painter) {
    painter->translate(boundingRect.x(), boundingRect.y());

    int w = boundingRect.width();
    int h = boundingRect.height();

    // 绘制 Indicator 的边框
    painter->setPen(QPen(Qt::darkGray, 2));
    painter->drawRect(0, 0, h, h);

    // 选中时，Indicator 内部绘制一个勾
    if (checked) {
        painter->setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(h*0.2, h*0.5,  h*0.4, h*0.75);
        painter->drawLine(h*0.4, h*0.75, h*0.9, h*0.3);
    }

    // 绘制 CheckBox 的文本
    painter->setPen(Qt::black);
    painter->drawText(h+10, 0, w-h-10, h, Qt::AlignLeft|Qt::AlignVCenter, text);
}

void CheckBox::mousePressed() {
    checked = !checked; // 鼠标按下时切换选中状态
    // 可以发射 statusChanged 信号
}
