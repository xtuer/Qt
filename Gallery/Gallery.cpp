#include "Gallery.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QGridLayout>
#include <QEvent>

/***********************************************************************************************************************
 *                                                    GalleryPrivate                                                   *
 **********************************************************************************************************************/
struct GalleryPrivate {
    GalleryPrivate(int imageWidth, int imageHeight, const QList<QString> &imagePaths);

    int  getPreviousImageIndex(); // 上一张图片的索引
    int  getNextImageIndex();     // 下一张图片的索引
    bool hasNextImage();          // 是否有上一张图片
    bool hasPreviousImage();      // 是否有下一张图片
    void showImage(int index);

    const int IMAGE_WIDTH;  // 图片的宽
    const int IMAGE_HEIGHT; // 图片的高
    const int IMAGE_COUNT;  // 图片的数量

    int currentImageIndex;   // 当前图片的索引
    int animationDuraton;    // 动画的播放时间，单位为毫秒

    QWidget *imageViewport;  // 能够看到图片的区域
    QWidget *imageContainer; // 所有图片所在的 widget
    QPushButton *prevButton; // 上一张按钮
    QPushButton *nextButton; // 下一张按钮
};

GalleryPrivate::GalleryPrivate(int imageWidth, int imageHeight, const QList<QString> &imagePaths)
    : IMAGE_WIDTH(imageWidth), IMAGE_HEIGHT(imageHeight), IMAGE_COUNT(imagePaths.size()), currentImageIndex(0) {
    animationDuraton = 200;
    int w = IMAGE_WIDTH;
    int h = IMAGE_HEIGHT;

    // viewport 的大小
    imageViewport = new QWidget();
    imageViewport->setFixedSize(w, h);

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

    // 上一张和下一张按钮
    int bw = 30;
    int bh = 20;
    prevButton = new QPushButton("<", imageViewport);
    nextButton = new QPushButton(">", imageViewport);
    prevButton->setFixedSize(bw, bh);
    nextButton->setFixedSize(bw, bh);
    prevButton->move(0, (h-bh)/2);
    nextButton->move((w-bw), (h-bh)/2);

    prevButton->setEnabled(false);
    prevButton->hide();
    nextButton->hide();

    // 点击 '上一张' 按钮显示前一张图片
    QObject::connect(prevButton, &QPushButton::clicked, [=] {
        showImage(getPreviousImageIndex());
        nextButton->setEnabled(hasNextImage());
        prevButton->setEnabled(hasPreviousImage());
    });

    // 点击 '下一张' 按钮显示下一张图片
    QObject::connect(nextButton, &QPushButton::clicked, [=] {
        showImage(getNextImageIndex());
        nextButton->setEnabled(hasNextImage());
        prevButton->setEnabled(hasPreviousImage());
    });

    imageViewport->setStyleSheet("QPushButton{border: 2px solid gray; background: #44000000; color: #EEE; font-weight: bold;}QPushButton:disabled{color:gray}");
    prevButton->setStyleSheet("border-left: 0 transparent; border-top-right-radius: 4px; border-bottom-right-radius: 4px;");
    nextButton->setStyleSheet("border-right: 0 transparent; background: #44000000; border-top-left-radius: 4px; border-bottom-left-radius: 4px;");
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

void GalleryPrivate::showImage(int index) {
    if (index >= 0 && index < IMAGE_COUNT) {
        // 使用动画显示图片，修改位置移动 d->imageContainer
        QPropertyAnimation *animation = new QPropertyAnimation(imageContainer, "pos");
        animation->setDuration(animationDuraton);
        animation->setEndValue(QPoint(-IMAGE_WIDTH * index, 0));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

/***********************************************************************************************************************
 *                                                       Gallery                                                       *
 * 使用图片的宽度，高度，路径创建 Gallery                                                                                   *
 * 为了显示效果，所有的图片的高宽最好都是 (imageWidth, imageHeight)                                                          *
 **********************************************************************************************************************/
Gallery::Gallery(int imageWidth, int imageHeight, const QList<QString> &imagePaths, QWidget *parent) : QWidget(parent) {
    d = new GalleryPrivate(imageWidth, imageHeight, imagePaths);
    d->imageViewport->installEventFilter(this);

    QGridLayout *gl = new QGridLayout();
    gl->setContentsMargins(0, 0, 0, 0);
    gl->addWidget(d->imageViewport, 0, 1);
    setLayout(gl);
}

int Gallery::getImageCount() const {
    return d->IMAGE_COUNT;
}

void Gallery::showImage(int index) const {
    d->showImage(index);
}

void Gallery::setAnimationDuration(int duration) {
    d->animationDuraton = duration;
}

bool Gallery::eventFilter(QObject *watched, QEvent *event) {
    // 鼠标进入 imageViewport 时显示上一张和下一张按钮，离开 imageViewport 时隐藏他们
    if (watched == d->imageViewport) {
        if (QEvent::Leave == event->type()) {
            d->prevButton->hide();
            d->nextButton->hide();
        } else if (QEvent::Enter == event->type()) {
            d->prevButton->show();
            d->nextButton->show();
        }
    }

    return QWidget::eventFilter(watched, event);
}
