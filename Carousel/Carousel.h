#ifndef CAROUSEL_H
#define CAROUSEL_H

#include <QWidget>
#include <QList>

class CarouselItem;
struct CarouselPrivate;

class Carousel : public QWidget {
    Q_OBJECT

public:
    explicit Carousel(int rotateRadius, int mostFrontItemWidth, int mostFrontItemHeight, int scaleDistanceToMostFrontItem,
                      const QList<QString> imagePaths, QWidget *parent = 0);
    ~Carousel();

    /**
     * @brief 旋转第 index 张图片到最前面
     * @param itemIndex 图片的下标
     */
    void rotateToItem(int itemIndex);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint calculateOrigin() const; // 计算绘图开始的原点坐标
    CarouselPrivate *d;
};

#endif // CAROUSEL_H
