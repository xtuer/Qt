#ifndef GALLERY_H
#define GALLERY_H
#include <QWidget>

struct GalleryPrivate;

/**
 * 图片轮播的 Widget
 */
class Gallery : public QWidget {
public:
    Gallery(int imageWidth, int imageHeight, const QList<QString> &imagePaths, QWidget *parent = NULL);

    int  getImageCount() const; // 获取图片的数量
    void showImage(int index) const; // 显示第 index 张图片
    void setAnimationDuration(int duration); // 设置动画播放时间

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    GalleryPrivate *d;
};

#endif // GALLERY_H
