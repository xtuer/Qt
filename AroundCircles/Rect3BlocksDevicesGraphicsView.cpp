#include "Rect3BlocksDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QTransform>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>

/*-----------------------------------------------------------------------------|
 |                   Rect3BlocksDevicesGraphicsViewPrivate                     |
 |----------------------------------------------------------------------------*/
class Rect3BlocksDevicesGraphicsViewPrivate {
    Rect3BlocksDevicesGraphicsViewPrivate();
    ~Rect3BlocksDevicesGraphicsViewPrivate();

    /**
     * 构建布点图
     */
    void buildCircleDevices();

    /**
     * @brief 创建设备布点
     * @param x  横坐标
     * @param y  纵坐标
     */
    void newDevice(double x, double y);

    /**
     * @brief 创建文本 item
     * @param text
     * @param x
     * @param y
     * @param font
     */
    void newText(QString text, double x, double y, QFont &font);

    QGraphicsScene *scene = nullptr;
    int radius = 25;  // 小圆的半径

    friend class Rect3BlocksDevicesGraphicsView;
};

Rect3BlocksDevicesGraphicsViewPrivate::Rect3BlocksDevicesGraphicsViewPrivate() {
    scene = new QGraphicsScene();
    buildCircleDevices();
}

Rect3BlocksDevicesGraphicsViewPrivate::~Rect3BlocksDevicesGraphicsViewPrivate() {
    delete scene;
}

// 创建矩形中的小圆
void Rect3BlocksDevicesGraphicsViewPrivate::buildCircleDevices() {
    // 2 行 10 列
    double r = radius; // 半径
    double R = r*2;    // 直径
    double p = 20;     // 矩形的边距
    double g = 15;     // 小圆的间隔
    double w = (R+g) * 10 - g + p*2;
    double h = (R+g) * 2 - g + p*2;

    double bh = h + p;
    double y0 = -(bh + h/2); // 最上面的 y

    // 有 3 个 blocks
    for (int n = 0; n < 3; n++, y0 += bh) {
        // 外边框
        QGraphicsRectItem *frame = new QGraphicsRectItem(-w/2, y0, w, h);
        scene->addItem(frame);

        for (int row = 0; row < 2; row++) {
            double x = -w/2 + p + r;
            double y = y0 + p + (R+g)*row + r;

            for (int col = 0; col < 10; col++, x += R+g) {
                newDevice(x, y);
            }
        }
    }

    // 创建 1-9 个文本
    QFont font;
    font.setPointSize(12);

    double tx1 = -w/2+r/2;
    double tx2 = tx1 + (R+g)*5;
    double tx3 = tx1 + (R+g)*10 - g;
    double ty1 = -(bh + h/2)+p/2 - 5;
    double ty2 = ty1 + bh;
    double ty3 = ty2 + bh;

    // 1, 8, 4
    newText("1", tx1, ty1, font);
    newText("8", tx1, ty2, font);
    newText("4", tx1, ty3+R+g, font);

    // 2, 7, 5
    newText("2", tx2-R-g, ty1, font);
    newText("7", tx2, ty2, font);
    newText("5", tx2-R-g, ty3, font);

    // 3, 9, 6
    newText("3", tx3, ty1, font);
    newText("9", tx3, ty2+R+g, font);
    newText("6", tx3, ty3+R+g, font);
}

void Rect3BlocksDevicesGraphicsViewPrivate::newDevice(double x, double y) {
    QTransform transform;
    transform.translate(x, y);

    QGraphicsEllipseItem *item = new CircleDevice("", "", radius);
    item->setTransform(transform);
    scene->addItem(item);
}

void Rect3BlocksDevicesGraphicsViewPrivate::newText(QString text, double x, double y, QFont &font) {
    QGraphicsTextItem *t = new QGraphicsTextItem(text);
    t->setPos(x, y);
    t->setFont(font);
    scene->addItem(t);
}

/*-----------------------------------------------------------------------------|
 |                          Rect3BlocksDevicesGraphicsView                     |
 |----------------------------------------------------------------------------*/

Rect3BlocksDevicesGraphicsView::Rect3BlocksDevicesGraphicsView(QWidget *parent) :
    QGraphicsView(parent), d(new Rect3BlocksDevicesGraphicsViewPrivate()) {
    setScene(d->scene);
}

Rect3BlocksDevicesGraphicsView::~Rect3BlocksDevicesGraphicsView() {
    delete d;
}
