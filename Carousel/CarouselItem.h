#ifndef CAROUSELITEM_H
#define CAROUSELITEM_H

#include <QRectF>
#include <QPixmap>
#include <QVector3D>

struct CarouselPrivate;

class CarouselItem {
public:
    CarouselItem(const QString &imagePath);

    /**
     * @brief 旋转 item
     *
     * @param angle         旋转的角度
     * @param axis          绕轴 axis 旋转
     * @param radius        旋转的半径
     * @param initWidth     不旋转时的宽，即旋转 0 度时的宽
     * @param initHeight    不旋转时的高，即旋转 0 度时的高
     * @param scaleDistance 用于计算缩放的距离，到旋转 0 度时 item 中心的距离
     */
    void rotate(double angle,
                const QVector3D &axis,
                int radius,
                int initWidth,
                int initHeight,
                int scaleDistance);

    QRectF    rect;   // item 的矩形区域
    QVector3D center; // item 的中心坐标
    QPixmap   image;  // item 要展示的图片
};
#endif // CAROUSELITEM_H
