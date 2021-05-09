#include "RectDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QTransform>
#include <QGraphicsRectItem>

/*-----------------------------------------------------------------------------|
 |                        RectDevicesGraphicsViewPrivate                       |
 |----------------------------------------------------------------------------*/
class RectDevicesGraphicsViewPrivate {
    RectDevicesGraphicsViewPrivate(int horizontalCount, int verticalCount);
    ~RectDevicesGraphicsViewPrivate();

    /**
     * 创建小圆
     */
    void buildCircleDevices();

    QGraphicsScene *scene = nullptr;

    int horizontalCount = 8; // 水平方向小圆的个数
    int verticalCount = 5; // 垂直方向小圆的个数
    int radius = 25; // 小圆的半径
    int padding = 10;
    int gap = 12;

    friend class RectDevicesGraphicsView;
};

RectDevicesGraphicsViewPrivate::RectDevicesGraphicsViewPrivate(int horizontalCount, int verticalCount) {
    this->horizontalCount = horizontalCount;
    this->verticalCount = verticalCount;

    scene = new QGraphicsScene();
    buildCircleDevices();
}

RectDevicesGraphicsViewPrivate::~RectDevicesGraphicsViewPrivate() {
    delete scene;
}

// 创建矩形中的小圆
void RectDevicesGraphicsViewPrivate::buildCircleDevices() {
    // 矩形的宽度: w = hc*2*radius + (hc-1)*gap + padding*2
    // 矩形的宽度: h = hc*2*radius + (vc-1)*gap + padding*2
    // 第一个小圆圆心的 x: -w/2+padding + radius
    // 第一个小圆圆心的 y: -h/2+padding + radius

    int hc = horizontalCount;
    int vc = verticalCount;
    double w = hc*2*radius + (hc-1)*gap + padding*2;
    double h = vc*2*radius + (vc-1)*gap + padding*2;

    // 外边框
    QGraphicsRectItem *frame = new QGraphicsRectItem(-w/2, -h/2, w, h);
    scene->addItem(frame);

    for (int v = 0; v < vc; v++) {
        double y = -h/2+padding + radius + v*(2*radius+gap);
        double x = -w/2+padding + radius;

        for (int h = 0; h < hc; h++) {
            QTransform transform;
            transform.translate(x, y);

            QGraphicsEllipseItem *item = new CircleDevice("", "", radius);
            item->setTransform(transform);
            scene->addItem(item);

            x += 2*radius+gap;
        }
    }
}

/*-----------------------------------------------------------------------------|
 |                            RectDevicesGraphicsView                          |
 |----------------------------------------------------------------------------*/
RectDevicesGraphicsView::RectDevicesGraphicsView(int horizontalCount, int verticalCount, QWidget *parent) :
    QGraphicsView(parent), d(new RectDevicesGraphicsViewPrivate(horizontalCount, verticalCount)) {
    setScene(d->scene);
}

RectDevicesGraphicsView::~RectDevicesGraphicsView() {
    delete d;
}
