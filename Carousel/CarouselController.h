#ifndef CAROUSELCONTROLLER_H
#define CAROUSELCONTROLLER_H
#include <QList>
#include <QObject>
#include <QRectF>
#include <QVector3D>

class CarouselItem;

class CarouselController : public QObject {
    Q_OBJECT
    Q_PROPERTY(double rotateAngle READ getRotateAngle WRITE setRotateAngle)

signals:
    void itemsRotated(); // items 被旋转了的信号

public:
    CarouselController(int maxItemWidth, int maxItemHeight, double minItemZoom, int rotateRadius, const QList<QString> imagePaths);
    ~CarouselController();

    double getRotateAngle() const;
    void   setRotateAngle(double rotateAngle); // 设置旋转角度，并把所有的 items 各自旋转 rotateAngle 度

    int  indexOfItemAt(const QPointF &pos) const;      // 返回在 pos 处的 item 的下标，如果没有找到则返回 -1
    void rotateItem(CarouselItem *item, double angle); // 把 item 旋转 angle 度，item 的 rect, center 会改变，但是 angle 值不变
    void rotateItemToFront(int index);                 // 旋转第 index 个 item 到最前面，变为最大的 item
    void calculateRects(); // 计算最小的 item(z 最小)、最大的 item(z 最大) 和所有 items 所在的矩形范围
    QList<CarouselItem *> sortedItemsByZ() const;      // 把所有的 items 按 z 坐标从小到大排序

    int       maxItemWidth;  // 最大的 item 的宽，最大的 item 也就是最前面的 item
    int       maxItemHeight; // 最大的 item 的高
    double    minItemZoom;   // 最小的 item 与最大的 item 的比例
    int       rotateRadius;  // 旋转的半径
    QVector3D rotateAxis;    // 旋转轴

    QRectF minRect;      // 最上面的 item 的 rect
    QRectF maxRect;      // 最前面的 item 的 rect
    QRectF boundingRect; // 所有 items 占据的矩形

    QList<CarouselItem *> carouselItems;
};

#endif // CAROUSELCONTROLLER_H
