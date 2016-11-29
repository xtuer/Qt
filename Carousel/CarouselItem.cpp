#include "CarouselItem.h"
#include "Carousel.h"
#include <QMatrix4x4>

CarouselItem::CarouselItem(const QString &imagePath)  : image(QPixmap(imagePath)) {

}

void CarouselItem::rotate(double angle,
                          const QVector3D &axis,
                          int radius,
                          int initWidth,
                          int initHeight,
                          int scaleDistance) {
    QMatrix4x4 matrix;
    matrix.rotate(angle, axis);

    // 没有旋转时图片的中心位置绕 axis 旋转后得到 item 的中心
    QVector3D initCenter(0, 0, radius);
    QVector3D center = matrix.map(initCenter);

    // 旋转后的中心
    this->center = center;

    // 计算 item 的矩形区域
    double rate = (center.z() - initCenter.z() + scaleDistance) / scaleDistance;
    double w = initWidth * rate;
    double h = initHeight * rate;
    this->rect.setRect(0, 0, w, h);
    this->rect.moveCenter(QPointF(center.x(), center.y()));
}
