#include "Rect16DevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QTransform>
#include <QGraphicsRectItem>

/*-----------------------------------------------------------------------------|
 |                        Rect16DevicesGraphicsViewPrivate                     |
 |----------------------------------------------------------------------------*/
class Rect16DevicesGraphicsViewPrivate {
    Rect16DevicesGraphicsViewPrivate();
    ~Rect16DevicesGraphicsViewPrivate();

    /**
     * 构建布点图
     */
    void buildCircleDevices();

    /**
     * @brief 创建设备布点
     * @param sn 序号
     * @param x  横坐标
     * @param y  纵坐标
     */
    void newDevice(int sn, double x, double y);

    QGraphicsScene *scene = nullptr;
    int width  = 600; // 布点范围的宽
    int height = 400; // 布点范围的高
    int radius = 25;  // 小圆的半径

    friend class Rect16DevicesGraphicsView;
};

Rect16DevicesGraphicsViewPrivate::Rect16DevicesGraphicsViewPrivate() {
    scene = new QGraphicsScene();
    buildCircleDevices();
}

Rect16DevicesGraphicsViewPrivate::~Rect16DevicesGraphicsViewPrivate() {
    delete scene;
}

// 创建矩形中的小圆
void Rect16DevicesGraphicsViewPrivate::buildCircleDevices() {
    // 外边框
    QGraphicsRectItem *frame = new QGraphicsRectItem(-width/2, -height/2, width, height);
    scene->addItem(frame);

    double padding = 8;
    double g  = 2;
    double w  = width - 2*padding;
    double h  = height - 2*padding;
    double r  = radius;
    double x1 = -w/2 + r;
    double x2 = -w/2 + r*3 + 5;
    double x3 = 0 - g*r;
    double x4 = 0 + g*r;
    double x5 = w/2 - r;
    double y1 = -h/2 + r;
    double y2 = r;
    double y3 = h/2 - r;

    // 1, 2, 3, 4, 5, 6 探头
    newDevice(1, x1, y1);
    newDevice(2, x1, y2);
    newDevice(3, x1, y3);
    newDevice(4, x2, y1);
    newDevice(5, x2, y2);
    newDevice(6, x2, y3);

    // 7, 8, 9, 11, 12, 13
    newDevice(7, x3, y1);
    newDevice(8, x3, y2 - g*r);
    newDevice(9, x3, y3);
    newDevice(11, x4, y1);
    newDevice(12, x4, y2 - g*r);
    newDevice(13, x4, y3);

    // 10
    newDevice(10, 0, y2);

    // 14, 15, 16
    newDevice(14, x5, y1);
    newDevice(15, x5, y2);
    newDevice(16, x5, y3);
}

void Rect16DevicesGraphicsViewPrivate::newDevice(int sn, double x, double y) {
    QTransform transform;
    transform.translate(x, y);

    QGraphicsEllipseItem *item = new CircleDevice(QString::number(sn), QString::number(sn), radius);
    item->setTransform(transform);
    scene->addItem(item);
}

/*-----------------------------------------------------------------------------|
 |                            Rect16DevicesGraphicsView                        |
 |----------------------------------------------------------------------------*/
Rect16DevicesGraphicsView::Rect16DevicesGraphicsView(QWidget *parent) :
    QGraphicsView(parent), d(new Rect16DevicesGraphicsViewPrivate()){
    setScene(d->scene);
}

Rect16DevicesGraphicsView::~Rect16DevicesGraphicsView() {
    delete d;
}
