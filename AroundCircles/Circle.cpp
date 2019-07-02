#include "Circle.h"

#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>

/*-----------------------------------------------------------------------------|
 |                                CenterCircle                                 |
 |----------------------------------------------------------------------------*/
CenterCircle::CenterCircle(int n, int radius, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem(rect, parent), n(n), radius(radius) {
}

void CenterCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::Antialiasing);

    // 圆上的序号: 逆时针，1 到 n
    for (int i = 0; i < n; ++i) {
        painter->save();
        painter->rotate(-360.0/n*i);
        painter->translate(0, radius-5);
        painter->drawText(QRectF(-8, -8, 16, 16), Qt::AlignCenter, QString::number(i+1));
        painter->restore();
    }

    painter->drawEllipse(rect());
}

/*-----------------------------------------------------------------------------|
 |                                AroundCircle                                 |
 |----------------------------------------------------------------------------*/
AroundCircle::AroundCircle(const QString &value, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem(rect, parent), value(value) {
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
}

void AroundCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(hover ? Qt::darkRed : Qt::black);
    painter->setBrush(bgcolor);
    painter->drawEllipse(rect());
    painter->drawText(rect(), Qt::AlignCenter, value);
}

// 设置背景色
void AroundCircle::setBgcolor(const QString &bgcolor) {
    QColor temp(bgcolor);
    this->bgcolor = temp.isValid() ? temp : Qt::transparent;
    update();
}

// 设置 scene 中名字为 name 的 AroundCircle 的 item 的背景色
void AroundCircle::setAroundCirleBgcolor(QGraphicsScene *scene, const QString &name, const QString &bgcolor) {
    for (QGraphicsItem *item : scene->items()) {
        if (int(CircleType::AroundCircle) == item->type()) {
            AroundCircle *circle = dynamic_cast<AroundCircle *>(item);

            if (name == circle->getName()) {
                circle->setBgcolor(bgcolor);
            }
        }
    }
}

// 重置 scene 中名字为 name 的圆的名字和背景色
void AroundCircle::resetAroundCircle(QGraphicsScene *scene, const QString &name) {
    // 重置同名圆的状态
    for (QGraphicsItem *item : scene->items()) {
        if (int(CircleType::AroundCircle) == item->type()) {
            AroundCircle *circle = dynamic_cast<AroundCircle *>(item);

            if (name == circle->name) {
                circle->reset();
            }
        }
    }
}

// 重置背景色和设置 name 为空
void AroundCircle::reset() {
    name    = "";
    bgcolor = Qt::transparent;
    update();
}

// 鼠标进入和离开、拖拽进入和离开时高亮圆
void AroundCircle::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hover = true;
    update();
}

void AroundCircle::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    hover = false;
    update();
}

void AroundCircle::dragEnterEvent(QGraphicsSceneDragDropEvent *) {
    hover = true;
    update();
}

void AroundCircle::dragLeaveEvent(QGraphicsSceneDragDropEvent *) {
    hover = false;
    update();
}

// 拖放时设置圆的背景色和名字
void AroundCircle::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("text/DnD-DEVICE-CIRCLE")) {
        event->accept();

        // 数据格式: name,color
        QStringList data = QString::fromUtf8(event->mimeData()->data("text/DnD-DEVICE-CIRCLE")).split(",");
        QString name = data.value(0);

        AroundCircle::resetAroundCircle(scene(), name);
        this->name = name;
        this->setBgcolor(data.value(1));
    }

    hover = false;
    update();
}


