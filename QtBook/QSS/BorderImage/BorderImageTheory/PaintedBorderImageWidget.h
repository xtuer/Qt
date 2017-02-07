#ifndef PAINTEDBORDERIMAGEWIDGET_H
#define PAINTEDBORDERIMAGEWIDGET_H

#include <QWidget>
#include <QList>

class PaintedBorderImageWidget : public QWidget {
    Q_OBJECT
public:
    explicit PaintedBorderImageWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void prepareBorderImages(const QString &imagePath);
    void drawBorderImages(QPainter *painter, const QRect &rect) const;
    QList<QRect> calculateNineRects(const QRect &rect,
                                    int top,
                                    int right,
                                    int bottom,
                                    int left) const;
    QPixmap scaleImage(const QPixmap &pixmap, const QSize &size) const;

    int top;    // 上边的宽
    int right;  // 右边的宽
    int bottom; // 下边的宽
    int left;   // 左边的宽

    QPixmap topLeftPixmap;      // 左上角的图片
    QPixmap topPixmap;          // 顶部的图片
    QPixmap topRightPixmap;     // 右上角的图片
    QPixmap leftPixmap;         // 左边的图片
    QPixmap centerPixmap;       // 中间的图片
    QPixmap rightPixmap;        // 右边的图片
    QPixmap bottomLeftPixmap;   // 左下角的图片
    QPixmap bottomPixmap;       // 底部的图片
    QPixmap bottomRightPixmap;  // 右下角的图片
};

#endif // PAINTEDBORDERIMAGEWIDGET_H
