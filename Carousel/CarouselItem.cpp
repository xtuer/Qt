#include "CarouselItem.h"
#include "Carousel.h"
#include <QMatrix4x4>

CarouselItem::CarouselItem(const QString &imagePath)  : image(QPixmap(imagePath)) {

}

void CarouselItem::rotate(double angle,
                          const QVector3D &rotateAxis,
                          int rotateRadius,
                          int initWidth,
                          int initHeight,
                          int scaleDistanceToMostFrontItem) {
    QMatrix4x4 matrix;
    matrix.rotate(angle, rotateAxis);

    // 最前面一张图片的中心位置绕 rotateAxis 旋转后得到 item 的中心
    QVector3D mostFrontItemCenter(0, 0, rotateRadius);
    QVector3D center = matrix.map(mostFrontItemCenter);

    // 旋转后的中心
    this->center = center;

    // 计算 item 的矩形区域
    double rate = (center.z() - mostFrontItemCenter.z() + scaleDistanceToMostFrontItem) / scaleDistanceToMostFrontItem;
    double w = initWidth * rate;
    double h = initHeight * rate;
    this->rect.setRect(0, 0, w, h);
    this->rect.moveCenter(QPointF(center.x(), center.y()));
}
