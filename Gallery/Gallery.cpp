#include "Gallery.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QGridLayout>

/***********************************************************************************************************************
 *                                                    GalleryPrivate                                                   *
 **********************************************************************************************************************/
struct GalleryPrivate {
    GalleryPrivate(int imageWidth, int imageHeight, const QList<QString> &imagePaths);

    int  getPreviousImageIndex(); // 上一张图片的索引
    int  getNextImageIndex();     // 下一张图片的索引
    bool hasNextImage();          // 是否有上一张图片
    bool hasPreviousImage();      // 是否有下一张图片

    const int IMAGE_WIDTH;  // 图片的宽
    const int IMAGE_HEIGHT; // 图片的高
    const int IMAGE_COUNT;  // 图片的数量

    int currentImageIndex;   // 当前图片的索引
    int animationDuraton;    // 动画的播放时间，单位为毫秒
    QWidget *imageViewport;  // 能够看到图片的区域
    QWidget *imageContainer; // 所有图片所在的 widget
};

GalleryPrivate::GalleryPrivate(int imageWidth, int imageHeight, const QList<QString> &imagePaths)
    : IMAGE_WIDTH(imageWidth), IMAGE_HEIGHT(imageHeight), IMAGE_COUNT(imagePaths.size()), currentImageIndex(0) {
    animationDuraton = 200;
    int w = IMAGE_WIDTH;
    int h = IMAGE_HEIGHT;

    // viewport 的大小
    imageViewport = new QWidget();
    imageViewport->resize(w, h);
    imageViewport->setMinimumSize(w, h);
    imageViewport->setMaximumSize(w, h);

    // imageContainer 的大小为所有图片排在一起的大小
    imageContainer = new QWidget(imageViewport);
    imageContainer->resize(w * IMAGE_COUNT, h);
    imageContainer->move(0, 0);

    // 每张图片放在一个 QLabel 中，QLabel 挨个排列在 imageContainer 上
    for (int i = 0; i < IMAGE_COUNT; ++i) {
        QLabel *label = new QLabel(imageContainer);
        label->setPixmap(QPixmap(imagePaths.at(i)));
        label->resize(w, h);
        label->move(w * i, 0);
    }
}

// 如果不是第一张，则 imageIndex 减 1，否则 imageIndex 不变
int GalleryPrivate::getPreviousImageIndex() {
    if (hasPreviousImage()) {
        --currentImageIndex;
    }

    return currentImageIndex;
}

// 如果不是最后一张，则 imageIndex 加 1，否则 imageIndex 不变
int GalleryPrivate::getNextImageIndex() {
    if (hasNextImage()) {
        ++currentImageIndex;
    }

    return currentImageIndex;
}

// 如果不是最后一张则返回 true，否则返回 false
bool GalleryPrivate::hasNextImage() {
    return currentImageIndex < IMAGE_COUNT - 1;
}

// 如果不是第一张则返回 true，否则返回 false
bool GalleryPrivate::hasPreviousImage() {
    return currentImageIndex > 0;
}

/***********************************************************************************************************************
 *                                                       Gallery                                                       *
 * 使用图片的宽度，高度，路径创建 Gallery                                                                                   *
 * 为了显示效果，所有的图片的高宽最好都是 (imageWidth, imageHeight)                                                          *
 **********************************************************************************************************************/
Gallery::Gallery(int imageWidth, int imageHeight, const QList<QString> &imagePaths, QWidget *parent) : QWidget(parent) {
    d = new GalleryPrivate(imageWidth, imageHeight, imagePaths);

    QGridLayout *gl = new QGridLayout();
    QPushButton *prevButton = new QPushButton("<");
    QPushButton *nextButton = new QPushButton(">");
    gl->addWidget(prevButton, 0, 0);
    gl->addWidget(d->imageViewport, 0, 1);
    gl->addWidget(nextButton, 0, 2);
    setLayout(gl);

    // 点击 '上一张' 按钮显示前一张图片
    connect(prevButton, &QPushButton::clicked, [=] {
        showImage(d->getPreviousImageIndex());
        nextButton->setEnabled(d->hasNextImage());
        prevButton->setEnabled(d->hasPreviousImage());
    });

    // 点击 '下一张' 按钮显示下一张图片
    connect(nextButton, &QPushButton::clicked, [=] {
        showImage(d->getNextImageIndex());
        nextButton->setEnabled(d->hasNextImage());
        prevButton->setEnabled(d->hasPreviousImage());
    });
}

int Gallery::getImageCount() const {
    return d->IMAGE_COUNT;
}

void Gallery::showImage(int index) const {
    if (index >= 0 && index < d->IMAGE_COUNT) {
        // 使用动画显示图片，修改位置移动 d->imageContainer
        QPropertyAnimation *animation = new QPropertyAnimation(d->imageContainer, "pos");
        animation->setDuration(d->animationDuraton);
        animation->setEndValue(QPoint(-d->IMAGE_WIDTH * index, 0));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void Gallery::setAnimationDuration(int duration) {
    d->animationDuraton = duration;
}
