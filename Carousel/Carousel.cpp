#include "Carousel.h"
#include "CarouselItem.h"
#include "CarouselController.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QtMath>
#include <QPropertyAnimation>

Carousel::Carousel(int maxItemWidth, int maxItemHeight, double minItemZoom, int rotateRadius,
                   const QList<QString> imagePaths, QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: #444;");

    d = new CarouselController(maxItemWidth, maxItemHeight, minItemZoom, rotateRadius, imagePaths);
    connect(d, SIGNAL(itemsRotated()), this, SLOT(update()));
}

Carousel::~Carousel() {
    delete d;
}

void Carousel::rotateItemToFront(int index) {
    d->rotateItemToFront(index);
}

void Carousel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.translate(calculateOrigin());

    // 从 z 最小的 item 开始绘制到 z 最大的 item，防止后面的 item 覆盖前面的 item
    QList<CarouselItem *> temp = d->sortedItemsByZ();

    foreach (const CarouselItem *item, temp) {
        painter.drawPixmap(item->rect.toRect(), item->pixmap);
    }
}

void Carousel::mousePressEvent(QMouseEvent *event) {
    // 绘制的中心是窗口的中心，点击的鼠标位置也要进行相应的偏移
    QPoint o = calculateOrigin();
    QPointF pos(event->x() - o.x(), event->y() - o.y());
    int clickedItemIndex = d->indexOfItemAt(pos);

    // 点击的 item 旋转到前面
    if (-1 != clickedItemIndex) {
        rotateItemToFront(clickedItemIndex);
        update();
    }
}

QPoint Carousel::calculateOrigin() const {
    int carouselHeight = (d->maxRect.bottom() - d->minRect.top());
    int x = width() / 2;
    int y = height() - (height() - carouselHeight) / 2 - d->maxRect.height() / 2;

    return QPoint(x, y);
}



