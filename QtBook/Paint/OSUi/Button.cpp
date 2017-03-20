#include "Button.h"

Button::Button(const QString &text, const QRect &boundingRect,
               const QColor &normalBackgroundColor,
               const QColor &hoverBackgroundColor,
               const QColor &pressedBackgroundColor)
    : Widget(boundingRect), text(text),
      normalBackgroundColor(normalBackgroundColor),
      hoverBackgroundColor(hoverBackgroundColor),
      pressedBackgroundColor(pressedBackgroundColor) {
}

void Button::paint(QPainter *painter) {
    QColor backgroundColor; // 按钮的背景色

    if (pressed) {
        backgroundColor = pressedBackgroundColor;
    } else if (hover) {
        backgroundColor = hoverBackgroundColor;
    } else {
        backgroundColor = normalBackgroundColor;
    }

    // 先绘制按钮的背景，然后居中绘制按钮的文本
    painter->setBrush(QBrush(backgroundColor));
    painter->drawRoundedRect(boundingRect, 2, 2);
    painter->drawText(boundingRect, Qt::AlignCenter, text);
}

