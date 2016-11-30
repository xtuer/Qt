#include "CarouselController.h"
#include "CarouselItem.h"
#include "Carousel.h"

#include <QMatrix4x4>
#include <QPropertyAnimation>

CarouselController::CarouselController(int rotateRadius, int frontItemWidth, int frontItemHeight,
                                       int scaleDistanceToFrontItem, const QList<QString> imagePaths)
        : rotateRadius(rotateRadius),
          frontItemWidth(frontItemWidth),
          frontItemHeight(frontItemHeight),
          scaleDistanceToFrontItem(scaleDistanceToFrontItem),
          rotateAxis(0, 1, -0.2) {
    // 初始化 items
    for (int i = 0; i < imagePaths.size(); ++i) {
        double rotateAngle = 360.0/imagePaths.size()*i;
        CarouselItem *item = new CarouselItem(0, imagePaths[i]);
        rotateItem(item, rotateAngle); // 旋转到 item 自己的位置上
        item->angle = rotateAngle;
        carouselItems << item;
    }

    // 计算 Carousel 相关的矩形
    calculateRects();
}

CarouselController::~CarouselController() {
    qDeleteAll(carouselItems);
}

double CarouselController::getRotateAngle() const {
    return 0; // 没啥用，只是 Q_PROPERTY 必须要有 READ
}

// 把所有的 items 旋转 rotateAngle 度
void CarouselController::setRotateAngle(double rotateAngle) {
    foreach (CarouselItem *item, carouselItems) {
        rotateItem(item, rotateAngle);
    }

    emit itemsRotated(); // 发射信号告知 items 旋转了
}

int CarouselController::indexOfItemAt(const QPointF &pos) const {
    int index = -1;
    double z = -10000000;

    // 找到在 pos 处最前面的 item，即在 pos 处且 z 最大
    for (int i = 0; i < carouselItems.size(); ++i) {
        if (carouselItems[i]->rect.contains(pos) && carouselItems[i]->center.z() > z) {
            index = i;
            z = carouselItems[i]->center.z();
        }
    }

    return index;
}

QList<CarouselItem *> CarouselController::sortedItemsByZ() const {
    QList<CarouselItem *> temp(carouselItems);

    // 传入 sort 需要的比较函数，按 z 坐标进行升序排列
    std::sort(temp.begin(), temp.end(), [](CarouselItem* a, CarouselItem* b) -> bool {
        return a->center.z() < b->center.z();
    });

    return temp;
}

void CarouselController::rotateItem(CarouselItem *item, double rotateAngle) {
    QMatrix4x4 matrix;
    matrix.rotate(item->angle + rotateAngle, rotateAxis);

    // 没有旋转时图片的中心位置绕 axis 旋转后得到 item 的中心
    QVector3D initCenter(0, 0, rotateRadius);
    QVector3D center = matrix.map(initCenter);

    // 旋转后的中心
    item->center = center;

    // 计算 item 的矩形区域
    double rate = (center.z() - initCenter.z() + scaleDistanceToFrontItem) / scaleDistanceToFrontItem;
    double w = frontItemWidth * rate;
    double h = frontItemHeight * rate;
    item->rect.setRect(0, 0, w, h);
    item->rect.moveCenter(QPointF(center.x(), center.y()));
}

void CarouselController::rotateItemToFront(int index) {
    const int itemCount = carouselItems.size();

    if (index >= itemCount) {
        qDebug() << QString("Index %1 out of bound %2").arg(index).arg(itemCount);
        return;
    }

    double rotateAngle = carouselItems[index]->angle;

    if (0 == rotateAngle) {
        return;
    }

    if (carouselItems[index]->center.x() > 0) {
        // 在右边，向左旋转
        rotateAngle = int(rotateAngle/360)*360 - rotateAngle;
    } else {
        // 在左边，向右旋转
        rotateAngle = int(rotateAngle/360+1)*360 - rotateAngle;
    }

    QPropertyAnimation *animation = new QPropertyAnimation(this, "rotateAngle");
    animation->setStartValue(0);
    animation->setEndValue(rotateAngle);
    animation->setDuration(qAbs(rotateAngle) / 180 * 1000); // 旋转时间和旋转角度成正比
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    connect(animation, &QPropertyAnimation::finished, [=] {
        // 旋转完后，设置 item 最终的角度
        foreach (CarouselItem *item, carouselItems) {
            item->angle += rotateAngle;

            // 校正角度在 [0, 360]
            if (item->angle > 360) {
                item->angle -= 360;
            }

            if (item->angle < 0) {
                item->angle += 360;
            }
        }
    });
}

void CarouselController::calculateRects() {
    QList<CarouselItem *> temp = sortedItemsByZ();
    minRect.setTopLeft(temp[0]->rect.topLeft());
    minRect.setBottomRight(temp[0]->rect.bottomRight());

    maxRect.setTopLeft(temp[temp.size()-1]->rect.topLeft());
    maxRect.setBottomRight(temp[temp.size()-1]->rect.bottomRight());

    double minX = 1000000, minY = 100000, maxX = -100000, maxY = -100000;
    foreach (const CarouselItem *item, carouselItems) {
        minX = qMin(minX, item->rect.x());
        minY = qMin(minY, item->rect.y());
        maxX = qMax(maxX, item->rect.right());
        maxY = qMax(maxY, item->rect.bottom());
    }

    carouselBoundingRect.setRect(minX, minY, maxX, maxY);
}
