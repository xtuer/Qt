#include "CarouselController.h"
#include "CarouselItem.h"

#include <QMatrix4x4>
#include <QPropertyAnimation>

CarouselController::CarouselController(int maxItemWidth, int maxItemHeight, double minItemZoom, int rotateRadius,
                                       const QList<QString> imagePaths)
        : maxItemWidth(maxItemWidth),
          maxItemHeight(maxItemHeight),
          minItemZoom(minItemZoom),
          rotateRadius(rotateRadius),
          rotateAxis(0, 1, -0.2) {
    // 初始化 items，把他们旋转到各自的位置
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
    double z = INT_MIN;

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
    QVector3D maxItemCenter(0, 0, rotateRadius);
    item->center = matrix.map(maxItemCenter);

    // 计算 item 的矩形区域
    double rate = minItemZoom + (1-minItemZoom) * (1 - (maxItemCenter.z() - item->center.z()) / 2 / rotateRadius);
    item->rect.setRect(0, 0, maxItemWidth * rate, maxItemHeight * rate);
    item->rect.moveCenter(item->center.toPointF());
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

    // 设置动画属性
    QPropertyAnimation *animation = new QPropertyAnimation(this, "rotateAngle");
    animation->setStartValue(0);
    animation->setEndValue(rotateAngle);
    animation->setDuration(qMax(300.0, qAbs(rotateAngle) / 180 * 800)); // 旋转的角度越大，时间越长
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

    double minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

    foreach (const CarouselItem *item, carouselItems) {
        minX = qMin(minX, item->rect.x());
        minY = qMin(minY, item->rect.y());
        maxX = qMax(maxX, item->rect.right());
        maxY = qMax(maxY, item->rect.bottom());
    }

    boundingRect.setRect(minX, minY, maxX, maxY);
}
