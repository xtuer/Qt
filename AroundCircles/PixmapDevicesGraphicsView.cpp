#include "PixmapDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QDropEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QResizeEvent>

/*-----------------------------------------------------------------------------|
 |                             DeviceGraphicsScene                             |
 |----------------------------------------------------------------------------*/
class DeviceGraphicsScene : public QGraphicsScene {
protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
};

// 接受拖拽
void DeviceGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    event->accept();
}

/*-----------------------------------------------------------------------------|
 |                      PixmapDevicesGraphicsViewPrivate                       |
 |----------------------------------------------------------------------------*/
class PixmapDevicesGraphicsViewPrivate {
    PixmapDevicesGraphicsViewPrivate();
    ~PixmapDevicesGraphicsViewPrivate();

    QGraphicsScene *scene = nullptr;
    friend class PixmapDevicesGraphicsView;
};

PixmapDevicesGraphicsViewPrivate::PixmapDevicesGraphicsViewPrivate() {
    scene = new DeviceGraphicsScene();
}

PixmapDevicesGraphicsViewPrivate::~PixmapDevicesGraphicsViewPrivate() {
}

/*-----------------------------------------------------------------------------|
 |                          PixmapDevicesGraphicsView                          |
 |----------------------------------------------------------------------------*/
PixmapDevicesGraphicsView::PixmapDevicesGraphicsView(QWidget *parent) : QGraphicsView(parent), d(new PixmapDevicesGraphicsViewPrivate) {
    setScene(d->scene);
    setAcceptDrops(true);
}

PixmapDevicesGraphicsView::~PixmapDevicesGraphicsView() {
    delete d;
}

void PixmapDevicesGraphicsView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/DnD-DEVICE-CIRCLE")) {
        event->accept();
    }
}

void PixmapDevicesGraphicsView::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat("text/DnD-DEVICE-CIRCLE")) {
        event->accept();

        // 数据格式: name,color
        QStringList data = QString::fromUtf8(event->mimeData()->data("text/DnD-DEVICE-CIRCLE")).split(",");
        QString name  = data.value(0);
        QString color = data.value(1);
        int w = 80;
        int h = 30;
        QPointF pos = mapToScene(event->pos() - QPoint(w/2, h/2));
        QRectF rect = QRectF(pos, QSize(w, h));

        d->scene->addItem(new RectDevice(name, name, color, rect));
    }
}

void PixmapDevicesGraphicsView::resizeEvent(QResizeEvent *event) {
    // Scene 的大小为 View 的大小
    setSceneRect(0, 0, event->size().width(), event->size().height());
}

