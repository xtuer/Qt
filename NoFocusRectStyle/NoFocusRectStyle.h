#ifndef NOFOCUSRECTSTYLE_H
#define NOFOCUSRECTSTYLE_H

#include <QProxyStyle>

class NoFocusRectStyle : public QProxyStyle {
public:
    NoFocusRectStyle(QStyle *baseStyle);
    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = 0) const;
};

#endif // NOFOCUSRECTSTYLE_H
