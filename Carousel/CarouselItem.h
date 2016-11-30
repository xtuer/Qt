#ifndef CAROUSELITEM_H
#define CAROUSELITEM_H

#include <QRectF>
#include <QVector3D>
#include <QPixmap>

/**
 * @brief 保存 Carousel item 的数据
 */
class CarouselItem {
public:
    CarouselItem(double angle, const QString &imagePath);

    double angle;     // 旋转的角度
    QRectF rect;      // item 所占据的矩形
    QVector3D center; // item 中心的坐标，是三维坐标，有 x, y, z
    QPixmap pixmap;   // item 要显示的 pixmap
};

#endif // CAROUSELITEM_H
