#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"

#include <QRect>
#include <QColor>
#include <QPainter>

/**
 * 按钮类，可以定义按钮的背景色，鼠标放在它上面时的背景色以及鼠标按下时的背景色等。
 */
class Button : public Widget {
public:
    Button(const QString &text, const QRect &boundingRect = QRect(0, 0, 100, 25),
           const QColor &normalBackgroundColor = QColor(200, 200, 200),
           const QColor &hoverBackgroundColor = QColor(200, 0, 200),
           const QColor &pressedBackgroundColor = QColor(0, 200, 200));

    void paint(QPainter *painter) Q_DECL_OVERRIDE;

    QString text; // 按钮的文本
    QColor  normalBackgroundColor;  // 背景色
    QColor  hoverBackgroundColor;   // 鼠标放到按钮上的背景色
    QColor  pressedBackgroundColor; // 鼠标按下按钮的背景色
};

#endif // BUTTON_H
