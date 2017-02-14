#include "NoFocusRectStyle.h"

NoFocusRectStyle::NoFocusRectStyle(QStyle *baseStyle) {

}

void NoFocusRectStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const {
    if (element == QStyle::PE_FrameFocusRect) {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
