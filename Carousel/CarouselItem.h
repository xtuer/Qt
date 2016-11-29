#ifndef CAROUSELITEM_H
#define CAROUSELITEM_H

#include <QRectF>
#include <QPixmap>
#include <QVector3D>

struct CarouselPrivate;

class CarouselItem {
public:
    CarouselItem(const QString &imagePath);
    void rotate(double angle,
                const QVector3D &rotateAxis,
                int rotateRadius,
                int initWidth,
                int initHeight,
                int scaleDistance);

    QRectF    rect;   // item 的矩形区域
    QVector3D center; // item 的中心坐标
    QPixmap   image;  // item 要展示的图片
};
#endif // CAROUSELITEM_H
