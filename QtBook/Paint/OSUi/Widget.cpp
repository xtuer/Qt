#include "Widget.h"

Widget::Widget(const QRect &boundingRect)
    : hover(false), pressed(false), boundingRect(boundingRect) {
}

void Widget::mouseMove() {

}

void Widget::mouseEnter() {
    hover = true;
}

void Widget::mouseLeave() {
    hover = false;
}

void Widget::mousePressed() {
    pressed = true;
}

void Widget::mouseReleased() {
    pressed = false;
}
