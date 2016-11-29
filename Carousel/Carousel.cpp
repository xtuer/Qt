#include "Carousel.h"
#include "CarouselItem.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QtMath>

/***********************************************************************************************************************
 *                                                   CarouselPrivate                                                   *
 **********************************************************************************************************************/
struct CarouselPrivate {
    CarouselPrivate(int rotateRadius, int mostFrontItemWidth, int mostFrontItemHeight, int scaleDistanceToMostFrontItem,
                    const QList<QString> &imagePaths);
    ~CarouselPrivate();

    int indexOfItem(const QPointF &pos) const;    // 返回包含 pos 的 item 的下标，如果没有找到，则返回 -1
    QList<CarouselItem *> sortedItemsByZ() const; // 把所有的 items 按 z 坐标从小到大排序
    void calculateRects(); // 计算 Carousel 的 items 中最小的 item(z 最小) 的 rect 和最大的 item(z 最大) 的 rect

    int rotateRadius;        // 旋转的半径
    int mostFrontItemWidth;  // 最前面 item 的宽
    int mostFrontItemHeight; // 最前面 item 的高
    int scaleDistanceToMostFrontItem; // 到最前面 item 的距离，用于缩放 items

    QRectF minRect; // 最上面的 item 的 rect
    QRectF maxRect; // 最前面的 item 的 rect
    QRectF carouselBoundingRect;

    QList<CarouselItem *> carouselItems;
};

CarouselPrivate::CarouselPrivate(int rotateRadius, int mostFrontItemWidth, int mostFrontItemHeight, int scaleDistanceToMostFrontItem,
                                 const QList<QString> &imagePaths)
    : rotateRadius(rotateRadius),
      mostFrontItemWidth(mostFrontItemWidth),
      mostFrontItemHeight(mostFrontItemHeight),
      scaleDistanceToMostFrontItem(scaleDistanceToMostFrontItem) {
    foreach (const QString &path, imagePaths) {
        carouselItems << new CarouselItem(path);
    }
}

CarouselPrivate::~CarouselPrivate() {
    qDeleteAll(carouselItems);
}

int CarouselPrivate::indexOfItem(const QPointF &pos) const {
    int index = -1;
    double z = -10000000;

    for (int i = 0; i < carouselItems.size(); ++i) {
        if (carouselItems[i]->rect.contains(pos) && carouselItems[i]->center.z() > z) {
            index = i;
            z = carouselItems[i]->center.z();
        }
    }

    return index;
}

QList<CarouselItem *> CarouselPrivate::sortedItemsByZ() const {
    QList<CarouselItem *> temp(carouselItems);

    // 传入 sort 需要的比较函数，按 z 坐标进行升序排列
    std::sort(temp.begin(), temp.end(), [](CarouselItem* a, CarouselItem* b) -> bool {
        return a->center.z() < b->center.z();
    });

    return temp;
}

void CarouselPrivate::calculateRects() {
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

/***********************************************************************************************************************
 *                                                      Carousel                                                       *
 **********************************************************************************************************************/
Carousel::Carousel(int rotateRadius, int mostFrontItemWidth, int mostFrontItemHeight, int scaleDistanceToMostFrontItem,
                   const QList<QString> imagePaths, QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: #444;");

    d = new CarouselPrivate(rotateRadius, mostFrontItemWidth, mostFrontItemHeight, scaleDistanceToMostFrontItem, imagePaths);
    rotateToItem(0);
    d->calculateRects();
}

Carousel::~Carousel() {
    delete d;
}

void Carousel::rotateToItem(int index) {
    const int itemCount = d->carouselItems.size();

    if (index >= itemCount) {
        qDebug() << QString("Index %1 out of bound %2").arg(index).arg(itemCount);
        return;
    }

    QVector3D rotateAxis(0, 1, -0.2); // 旋转的轴

    for (int i = 0; i < itemCount; ++i, index = (index+1)%itemCount) {
        d->carouselItems[index]->rotate(360.0 / itemCount * i,
                                        rotateAxis,
                                        d->rotateRadius,
                                        d->mostFrontItemWidth,
                                        d->mostFrontItemHeight,
                                        d->scaleDistanceToMostFrontItem);
    }
}

void Carousel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.translate(calculateOrigin());

    QList<CarouselItem *> temp = d->sortedItemsByZ();

    // 绘制，只是一种显示形式
    foreach (const CarouselItem *item, temp) {
        int x = item->rect.x();
        int y = item->rect.y();
        int w = item->rect.width();
        int h = item->rect.height();
        painter.drawPixmap(x, y, w, h, item->image);
    }
}

void Carousel::mousePressEvent(QMouseEvent *event) {
    // 绘制的中心是窗口的中心，点击的鼠标位置也要进行相应的偏移
    QPoint o = calculateOrigin();
    QPointF pos(event->x() - o.x(), event->y() - o.y());
    int clickedItemIndex = d->indexOfItem(pos);

    // 旋转点击的 item
    if (-1 != clickedItemIndex) {
        rotateToItem(clickedItemIndex);
        update();
    }
}

QPoint Carousel::calculateOrigin() const {
    int carouselHeight = (d->maxRect.bottom() - d->minRect.top());
    int x = width() / 2;
    int y = height() - (height() - carouselHeight) / 2 - d->maxRect.height() / 2;

    return QPoint(x, y);
}



