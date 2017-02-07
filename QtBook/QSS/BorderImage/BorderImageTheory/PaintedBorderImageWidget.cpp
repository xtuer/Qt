#include "PaintedBorderImageWidget.h"
#include <QPainter>
#include <QPixmap>

PaintedBorderImageWidget::PaintedBorderImageWidget(QWidget *parent) : QWidget(parent) {
    // 边宽，我们这里选择的图片比较简单，边宽都是 12px
    top    = 12;
    right  = 12;
    bottom = 12;
    left   = 12;

    // 背景图存储在资源文件里，当然也可以是文件系统里的文件
    prepareBorderImages(":/img/round-button.png");
}

void PaintedBorderImageWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    drawBorderImages(&painter, rect()); // 绘制的区域是整个 Widget
}

void PaintedBorderImageWidget::prepareBorderImages(const QString &imagePath) {
    // 加载背景图
    QPixmap pixmap(imagePath);

    // 把圆角背景图分割成 9 个部分
    QRect pixmapRect(0, 0, pixmap.width(), pixmap.height());
    QList<QRect> rects = calculateNineRects(pixmapRect, top, right, bottom, left);

    topLeftPixmap  = pixmap.copy(rects.at(0));
    topPixmap      = pixmap.copy(rects.at(1));
    topRightPixmap = pixmap.copy(rects.at(2));

    leftPixmap   = pixmap.copy(rects.at(3));
    centerPixmap = pixmap.copy(rects.at(4));
    rightPixmap  = pixmap.copy(rects.at(5));

    bottomLeftPixmap  = pixmap.copy(rects.at(6));
    bottomPixmap      = pixmap.copy(rects.at(7));
    bottomRightPixmap = pixmap.copy(rects.at(8));
}

void PaintedBorderImageWidget::drawBorderImages(QPainter *painter, const QRect &rect) const {
    // 把要绘制的 Rect 分割成 9 个部分，上，右，下，左 4 边的宽和背景图的一样
    QList<QRect> rects = calculateNineRects(rect, top, right, bottom, left);

    QRect topLeftRect  = rects.at(0);
    QRect topRect      = rects.at(1);
    QRect topRightRect = rects.at(2);

    QRect leftRect   = rects.at(3);
    QRect centerRect = rects.at(4);
    QRect rightRect  = rects.at(5);

    QRect bottomLeftRect  = rects.at(6);
    QRect bottomRect      = rects.at(7);
    QRect bottomRightRect = rects.at(8);

    // 把背景图片上的 9 个部分使用缩放的方式绘制到 Rect 上对应的 9 个部分
    painter->drawPixmap(topLeftRect, scaleImage(topLeftPixmap, topLeftRect.size()));
    painter->drawPixmap(topRect, scaleImage(topPixmap, topRect.size()));
    painter->drawPixmap(topRightRect, scaleImage(topRightPixmap,topRightRect.size()));

    painter->drawPixmap(leftRect, scaleImage(leftPixmap, leftRect.size()));
    painter->drawPixmap(centerRect, scaleImage(centerPixmap, centerRect.size()));
    painter->drawPixmap(rightRect, scaleImage(rightPixmap, rightRect.size()));

    painter->drawPixmap(bottomLeftRect, scaleImage(bottomLeftPixmap, bottomLeftRect.size()));
    painter->drawPixmap(bottomRect, scaleImage(bottomPixmap, bottomRect.size()));
    painter->drawPixmap(bottomRightRect, scaleImage(bottomRightPixmap, bottomRightRect.size()));
}

QList<QRect> PaintedBorderImageWidget::calculateNineRects(const QRect &rect,
                                                          int top,
                                                          int right,
                                                          int bottom,
                                                          int left) const {
    // 根据给定的 4 个边的宽和高，把 Rect 从上到下，从左到右分割成 9 个部分
    int x = rect.x();
    int y = rect.y();
    int cw = rect.width() - left - right;
    int ch = rect.height() - top - bottom;

    QRect topLeftRect(x, y, left, top);
    QRect topRect(x + left, y, cw, top);
    QRect topRightRect(x + left + cw, y, right, top);

    QRect leftRect(x, y + top, left, ch);
    QRect centerRect(x + left, y + top, cw, ch);
    QRect rightRect(x + left + cw, y + top, right, ch);

    QRect bottomLeftRect(x, y + top + ch, left, bottom);
    QRect bottomRect(x + left, y + top + ch, cw, bottom);
    QRect bottomRightRect(x + left + cw, y + top + ch, right, bottom);

    return QList<QRect>() << topLeftRect << topRect << topRightRect
                          << leftRect << centerRect << rightRect
                          << bottomLeftRect << bottomRect << bottomRightRect;
}

QPixmap PaintedBorderImageWidget::scaleImage(const QPixmap &pixmap, const QSize &size) const {
    // 缩放的时候忽略图片的高宽比，使用平滑缩放的效果
    return pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}
