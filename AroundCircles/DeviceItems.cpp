#include "DeviceItems.h"

#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>

/*-----------------------------------------------------------------------------|
 |                                  DialPlate                                  |
 |----------------------------------------------------------------------------*/
DialPlate::DialPlate(int n, double radius, int p, QGraphicsItem *parent)
    : QGraphicsEllipseItem(QRectF(-p-radius, -p-radius, p+p+radius+radius, p+p+radius+radius), parent), n(n), radius(radius) {
}

void DialPlate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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

    // 画圆
    painter->drawEllipse(rect());
}


/*-----------------------------------------------------------------------------|
 |                                CircleDevice                                 |
 |----------------------------------------------------------------------------*/
CircleDevice::CircleDevice(const QString &value, double radius, QGraphicsItem *parent)
    : QGraphicsEllipseItem(QRectF(-radius, -radius, radius+radius, radius+radius), parent), value(value) {
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
}

// 设置背景色
void CircleDevice::setBgcolor(const QString &bgcolor) {
    QColor temp(bgcolor);
    this->bgcolor = temp.isValid() ? temp : Qt::transparent;
    update();
}

// 设置 scene 中名字为 name 的 CircleDevice 的 item 的背景色
void CircleDevice::setBgcolorByName(QGraphicsScene *scene, const QString &name, const QString &bgcolor) {
    for (QGraphicsItem *item : scene->items()) {
        if (int(ItemType::CIRCLE_DEVICE) == item->type()) {
            CircleDevice *circle = dynamic_cast<CircleDevice *>(item);

            if (name == circle->getName()) {
                circle->setBgcolor(bgcolor);
            }
        }
    }
}

// 重置背景色和名字
void CircleDevice::reset() {
    name    = "";
    bgcolor = Qt::transparent;
    update();
}

// 重置 scene 中名字为 name 的圆的名字和背景色
void CircleDevice::resetByName(QGraphicsScene *scene, const QString &name) {
    // 重置同名圆的状态
    for (QGraphicsItem *item : scene->items()) {
        if (int(ItemType::CIRCLE_DEVICE) == item->type()) {
            CircleDevice *circle = dynamic_cast<CircleDevice *>(item);

            if (name == circle->name) {
                circle->reset();
            }
        }
    }
}

// 鼠标进入和离开、拖拽进入和离开时高亮圆
void CircleDevice::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hover = true;
    update();
}

void CircleDevice::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    hover = false;
    update();
}

void CircleDevice::dragEnterEvent(QGraphicsSceneDragDropEvent *) {
    hover = true;
    update();
}

void CircleDevice::dragLeaveEvent(QGraphicsSceneDragDropEvent *) {
    hover = false;
    update();
}

// 拖放时设置设备的背景色和名字
void CircleDevice::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("text/DnD-DEVICE-CIRCLE")) {
        event->accept();

        // 数据格式: name,color
        QStringList data = QString::fromUtf8(event->mimeData()->data("text/DnD-DEVICE-CIRCLE")).split(",");
        QString name = data.value(0);

        CircleDevice::resetByName(scene(), name);
        this->name = name;
        this->setBgcolor(data.value(1));
    }

    hover = false;
    update();
}

void CircleDevice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(hover ? Qt::darkRed : Qt::black);
    painter->setBrush(bgcolor);
    painter->drawEllipse(rect());
    painter->drawText(rect(), Qt::AlignCenter, value);
}


/*-----------------------------------------------------------------------------|
 |                                 RectDevice                                  |
 |----------------------------------------------------------------------------*/
RectDevice::RectDevice(const QString &name, const QString &value, const QString &bgcolor, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent), name(name), value(value), bgcolor(bgcolor) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
}

void RectDevice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::Antialiasing);

    // 选中是使用虚线边框
    if (isSelected()) {
        QPen pen = painter->pen();
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
    }

    // 绘制圆角矩形
    painter->setBrush(bgcolor);
    painter->drawRoundedRect(rect(), 4, 4);

    // 绘制文本
    painter->setPen(Qt::black);
    painter->drawText(rect(), Qt::AlignCenter, value);
}
