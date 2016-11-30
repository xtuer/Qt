#ifndef CAROUSELCONTROLLER_H
#define CAROUSELCONTROLLER_H
#include <QList>
#include <QObject>
#include <QRectF>
#include <QVector3D>

class Carousel;
class CarouselItem;

class CarouselController : public QObject {
    Q_OBJECT
    Q_PROPERTY(double rotateAngle READ getRotateAngle WRITE setRotateAngle)

signals:
    void itemsRotated();

public:
    CarouselController(int rotateRadius, int frontItemWidth, int frontItemHeight,
                       int scaleDistanceToFrontItem, const QList<QString> imagePaths);
    ~CarouselController();

    double getRotateAngle() const;
    void   setRotateAngle(double rotateAngle); // 设置旋转角度，旋转所有的 items rotateAngle 度

    int  indexOfItemAt(const QPointF &pos) const;      // 返回在 pos 处的 item 的下标，如果没有找到则返回 -1
    void rotateItem(CarouselItem *item, double angle); // 把 item 旋转 angle 度
    void rotateItemToFront(int index);                 // 旋转第 index 个 item 到最前面
    void calculateRects(); // 计算 Carousel 的 items 中最小的 item(z 最小) 的 rect 和最大的 item(z 最大) 的 rect
    QList<CarouselItem *> sortedItemsByZ() const;      // 把所有的 items 按 z 坐标从小到大排序

    int rotateRadius;     // 旋转的半径
    int frontItemWidth;   // 最前面 item 的宽
    int frontItemHeight;  // 最前面 item 的高
    int scaleDistanceToFrontItem; // 到最前面 item 的距离，用于缩放 items
    QVector3D rotateAxis; // 旋转轴

    QRectF minRect; // 最上面的 item 的 rect
    QRectF maxRect; // 最前面的 item 的 rect
    QRectF carouselBoundingRect;

    QList<CarouselItem *> carouselItems;
};

#endif // CAROUSELCONTROLLER_H
