#include "OSUi.h"
#include "Button.h"
#include "CheckBox.h"

#include <QRect>
#include <QPainter>
#include <QMouseEvent>

OSUi::OSUi(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);

    // 创建界面上的所有控件
    widgets << new Button("按钮一", QRect(20, 20, 100, 25))
            << new Button("按钮二", QRect(20, 70, 100, 25), QColor(200, 200, 200), QColor(200, 200, 0), QColor(0, 200, 0))
            << new CheckBox("生杀大权在你手里", true, QRect(150, 50, 150, 25));
}

OSUi::~OSUi() {
    qDeleteAll(widgets);
}

void OSUi::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 每次更新都需要绘制所有的控件
    foreach (Widget *w, widgets) {
        painter.save();
        w->paint(&painter);
        painter.restore();
    }
}

void OSUi::mouseMoveEvent(QMouseEvent *event) {
    foreach (Widget *w, widgets) {
        if (w->boundingRect.contains(event->pos())) {
            w->mouseMove();

            // 当鼠标在 widget 上移动时，如果 widget 的 hover 为 false，则触发一次 mouseEnter 事件，告知鼠标进入它了
            if (!w->hover) {
                w->mouseEnter();
            }
        } else {
            // 当鼠标不在 widget 上移动时，如果 widget 的 hover 为 true，则触发一次 mouseLeave 事件，告知鼠标已经离开它了
            if (w->hover) {
                w->mouseLeave();
            }
        }
    }

    update();
}

void OSUi::mousePressEvent(QMouseEvent *event) {
    foreach (Widget *w, widgets) {
        if (w->boundingRect.contains(event->pos())) {
            w->mousePressed();
        }
    }

    update();
}

void OSUi::mouseReleaseEvent(QMouseEvent *) {
    foreach (Widget *w, widgets) {
        if (w->pressed) {
            w->mouseReleased();
        }
    }

    update();
}
