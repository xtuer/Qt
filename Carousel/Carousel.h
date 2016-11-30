#ifndef CAROUSEL_H
#define CAROUSEL_H

#include <QWidget>
#include <QList>

class CarouselController;

class Carousel : public QWidget {
    Q_OBJECT

public:
    explicit Carousel(int maxItemWidth, int maxItemHeight, double minZoom, int rotateRadius,
                      const QList<QString> imagePaths, QWidget *parent = 0);
    ~Carousel();

    /**
     * @brief 旋转第 index 张图片到最前面
     * @param itemIndex 图片的下标
     */
    void rotateItemToFront(int itemIndex);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint calculateOrigin() const; // 计算绘图开始的原点坐标
    CarouselController *d;
};

#endif // CAROUSEL_H
