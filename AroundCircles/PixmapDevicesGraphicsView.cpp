#include "PixmapDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QDropEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QResizeEvent>
#include <QFileDialog>

/*-----------------------------------------------------------------------------|
 |                             DeviceGraphicsScene                             |
 |----------------------------------------------------------------------------*/
/**
 * 自定义 Scene，为了使得 Graphics View 能够接受拖放事件
 */
class DeviceGraphicsScene : public QGraphicsScene {
protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
};

// Scene 先接受拖拽，然后 view 才能处理 dropEvent
void DeviceGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    event->accept();
}

/*-----------------------------------------------------------------------------|
 |                      PixmapDevicesGraphicsViewPrivate                       |
 |----------------------------------------------------------------------------*/
class PixmapDevicesGraphicsViewPrivate {
    PixmapDevicesGraphicsViewPrivate();
    ~PixmapDevicesGraphicsViewPrivate();

    // 缩放图片，在 scene 中居中显示
    void scalePixmapAndCenterInScene(int w, int h);

    // 删除选择图片的提示
    void removeTip();

    QPixmap pixmap;
    QGraphicsScene      *scene = nullptr;
    QGraphicsPixmapItem *pixmapItem;  // 图片的 item
    QGraphicsTextItem   *tipTextItem; // 提示的 item

    friend class PixmapDevicesGraphicsView; // 友元是为了可以访问私有成员
};

PixmapDevicesGraphicsViewPrivate::PixmapDevicesGraphicsViewPrivate() {
    // pixmap.load("/Users/Biao/Pictures/bridge.jpg"); // 默认加载图片，方便测试
    pixmapItem  = new QGraphicsPixmapItem();
    tipTextItem = new QGraphicsTextItem("双击选择图片");

    scene = new DeviceGraphicsScene();
    scene->addItem(pixmapItem);
    scene->addItem(tipTextItem);
}

PixmapDevicesGraphicsViewPrivate::~PixmapDevicesGraphicsViewPrivate() {
    delete scene;
}

// 缩放图片，在 scene 中居中显示
void PixmapDevicesGraphicsViewPrivate::scalePixmapAndCenterInScene(int w, int h) {
    if (pixmap.isNull() || w<=0 || h<=0) { return; }

    // 等比缩放 pixmap
    QPixmap scaled = pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int x = (w - scaled.width()) / 2;
    int y = (h - scaled.height()) / 2;

    pixmapItem->setPos(x, y);
    pixmapItem->setPixmap(scaled);
}

// 删除选择图片的提示
void PixmapDevicesGraphicsViewPrivate::removeTip() {
    if (tipTextItem != nullptr) {
        delete tipTextItem;
        tipTextItem = nullptr;
    }
}

/*-----------------------------------------------------------------------------|
 |                          PixmapDevicesGraphicsView                          |
 |----------------------------------------------------------------------------*/
// 在图片上布点设备的 view，双击 view 选择背景图，拖放设备到 view 上创建设备的 item。
PixmapDevicesGraphicsView::PixmapDevicesGraphicsView(QWidget *parent) : QGraphicsView(parent), d(new PixmapDevicesGraphicsViewPrivate) {
    setScene(d->scene);
    setAcceptDrops(true);
}

PixmapDevicesGraphicsView::~PixmapDevicesGraphicsView() {
    delete d;
}

// 拖拽进入时接受拖拽事件
void PixmapDevicesGraphicsView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/DnD-DEVICE-CIRCLE")) {
        event->accept();
    }
}

// 拖放时创建 device item
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

        // 如果已经存在同名的 item，先删除掉它，然后添加新的
        delete findDeviceItemByName(d->scene, name);

        // 添加新的 item
        d->scene->addItem(new RectDevice(name, name, color, rect));
    }
}

// 窗口大小时修改 scene 和背景图的大小
void PixmapDevicesGraphicsView::resizeEvent(QResizeEvent *event) {
    int w = event->size().width();
    int h = event->size().height();

    setSceneRect(0, 0, w, h);             // Scene 的大小为 View 的大小
    d->scalePixmapAndCenterInScene(w, h); // 缩放图片，在 scene 中居中显示
}

// 双击选择图片
void PixmapDevicesGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    // 1. 双击弹出文件选择对话框选择图片
    // 2. 缩放图片，在 scene 中居中显示
    // 3. 第一次选择图片后删除提示选择图片的 item
    event->accept();

    // [1] 双击弹出文件选择对话框选择图片
    QString imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("Images (*.png *.jpg *.jpeg)"));

    if (!imagePath.isEmpty()) {
        // [2] 缩放图片，在 scene 中居中显示
        d->pixmap.load(imagePath);
        d->scalePixmapAndCenterInScene(width(), height());

        // [3] 第一次选择图片后删除提示选择图片的 item
        d->removeTip();
    }
}

