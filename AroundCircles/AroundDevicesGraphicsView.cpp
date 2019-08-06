#include "AroundDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QDebug>
#include <QtMath>
#include <QTransform>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

/*-----------------------------------------------------------------------------|
 |                      AroundDevicesGraphicsViewPrivate                       |
 |----------------------------------------------------------------------------*/
class AroundDevicesGraphicsViewPrivate {
    AroundDevicesGraphicsViewPrivate();
    ~AroundDevicesGraphicsViewPrivate();

    /**
     * 计算紧紧围绕半径为 r 的大圆的 n 个小圆的半径 (大圆和小圆相切，小圆之间相切)
     *
     * @param r 大圆的半径
     * @param n 小圆的个数
     * @return 返回小圆的半径
     */
    double calculateCircleDeviceRadius(double r, int n) const;

    /**
     * 创建绕大圆的小圆
     */
    void buildCircleDevices();

    QGraphicsScene *scene = nullptr;
    int n  = 16;  // 绕大圆的第一圈小圆的个数
    int cr = 100; // 中心大圆的半径

    friend class AroundDevicesGraphicsView;
};

AroundDevicesGraphicsViewPrivate::AroundDevicesGraphicsViewPrivate() {
    scene = new QGraphicsScene();
    buildCircleDevices();
}

AroundDevicesGraphicsViewPrivate::~AroundDevicesGraphicsViewPrivate() {
    delete scene;
}

// 计算紧紧围绕半径为 r 的大圆的 n 个小圆的半径 (大圆和小圆相切，小圆之间相切)
double AroundDevicesGraphicsViewPrivate::calculateCircleDeviceRadius(double r, int n) const {
    // 圆的内切正 n 边型的边长为 2*r*sin(π/n)
    // 小圆的半径为 x, 计算半径为 r+x 的圆的 2n 个内切正多边形的边长 b, b 等于 x
    double t = qSin(M_PI/2/n);
    return (2*r*t) / (1-2*t);
}

// 创建绕大圆的小圆
void AroundDevicesGraphicsViewPrivate::buildCircleDevices() {
    // 1. 计算小圆的半径
    // 2. 创建第一圈绕大圆的小圆
    // 3. 创建第二圈绕大圆的小圆
    // 4. 创建大圆

    // [1] 计算小圆的半径
    double sr = calculateCircleDeviceRadius(cr, n);
    int    sn = 1; // 小圆的序号，作为小圆的 id

    // [2] 创建第一圈绕大圆的小圆
    double br1 = cr+sr;
    int    n1  = n; // 第一圈小圆的个数

    for (int i = 0; i < n1; ++i) {
        QTransform transform;
        transform.rotate(-360.0/n1*i);
        transform.translate(0, br1+15);

        QGraphicsEllipseItem *item = new CircleDevice(QString::number(sn++), sr);
        item->setTransform(transform);
        scene->addItem(item);
    }

    // [3] 创建第二圈绕大圆的小圆
    double br2 = cr+sr*3;
    int    n2  = int(M_PI*br2 / sr); // 第二圈小圆的个数

    for (int i = 0; i < n2; ++i) {
        QTransform transform;
        transform.rotate(-360.0/n2*i);
        transform.translate(0, br2+20);

        QGraphicsEllipseItem *item = new CircleDevice(QString::number(sn++), sr);
        item->setTransform(transform);
        scene->addItem(item);
    }

    // [4] 创建大圆
    int padding = 8; // padding 为 8 调整效果
    scene->addItem(new DialPlate(16, cr, padding));
}

/*-----------------------------------------------------------------------------|
 |                          AroundDevicesGraphicsView                          |
 |----------------------------------------------------------------------------*/
AroundDevicesGraphicsView::AroundDevicesGraphicsView(QWidget *parent)
    : QGraphicsView(parent), d(new AroundDevicesGraphicsViewPrivate()) {
    setScene(d->scene);
}

AroundDevicesGraphicsView::~AroundDevicesGraphicsView() {
    delete d;
}

// 设置名字为传入的 name 的圆的背景色，圆的名字在拖放时自动设置
void AroundDevicesGraphicsView::setCircleDeviceBgcolor(const QString &name, const QString &bgcolor) {
    CircleDevice::setBgcolorByName(scene(), name, bgcolor);
}
