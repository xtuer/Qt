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
    CarouselController(int rotateRadius, int frontItemWidth, int frontItemHeight,
                       double minZoom, const QList<QString> imagePaths);
    ~CarouselController();

    double getRotateAngle() const;
    void   setRotateAngle(double rotateAngle); // 设置旋转角度，把所有的 items 各自旋转 rotateAngle 度

    int  indexOfItemAt(const QPointF &pos) const;      // 返回在 pos 处的 item 的下标，如果没有找到则返回 -1
    void rotateItem(CarouselItem *item, double angle); // 把 item 旋转 angle 度
    void rotateItemToFront(int index);                 // 旋转第 index 个 item 到最前面
    void calculateRects(); // 计算最小的 item(z 最小)、最大的 item(z 最大) 和所有 items 所在的矩形范围
    QList<CarouselItem *> sortedItemsByZ() const;      // 把所有的 items 按 z 坐标从小到大排序

    int rotateRadius;     // 旋转的半径
    int frontItemWidth;   // 最前面 item 的宽
    int frontItemHeight;  // 最前面 item 的高
    double minZoom;       // 最小的 item 与最前面 item 的比例
    QVector3D rotateAxis; // 旋转轴

    QRectF minRect; // 最上面的 item 的 rect
    QRectF maxRect; // 最前面的 item 的 rect
    QRectF carouselBoundingRect;

    QList<CarouselItem *> carouselItems;
};

#endif // CAROUSELCONTROLLER_H
