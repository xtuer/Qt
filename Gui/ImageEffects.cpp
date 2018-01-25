#include "ImageEffects.h"

// 灰度效果(grayscale)就是取红、绿、蓝三个像素值的算术平均值，这实际上将图像转成了黑白形式。
QImage ImageEffects::gray(QImage image) {
    map(&image, [](int r, int g, int b, int a) -> QRgb {
        int nr = (r + g + b) / 3; // qGray(r, g, b);
        int ng = nr;
        int nb = nr;

        return qRgba(nr, ng, nb, a);
    });

    return image;
}

// 复古效果(sepia)则是将红、绿、蓝三个像素，分别取这三个值的某种加权平均值，使得图像有一种古旧的效果。
QImage ImageEffects::siepa(QImage image) {
    map(&image, [](int r, int g, int b, int a) -> QRgb {
        int nr = (r * 0.393) + (g * 0.769) + (b * 0.189); // red
        int ng = (r * 0.349) + (g * 0.686) + (b * 0.168); // green
        int nb = (r * 0.272) + (g * 0.534) + (b * 0.131); // blue

        return qRgba(nr, ng, nb, a);
    });

    return image;
}

// 红色蒙版指的是，让图像呈现一种偏红的效果。算法是将红色通道设为红、绿、蓝三个值的平均值，而将绿色通道和蓝色通道都设为 0。
QImage ImageEffects::red(QImage image) {
    map(&image, [](int r, int g, int b, int a) -> QRgb {
        int nr = (r + g + b) / 3; // red
        int ng = 0; // green
        int nb = 0; // blue

        return qRgba(nr, ng, nb, a);
    });

    return image;
}

// 反转效果(invert)是指图片呈现一种色彩颠倒的效果。算法为红、绿、蓝通道都取各自的相反值(255-原值)。
QImage ImageEffects::invert(QImage image) {
    map(&image, [](int r, int g, int b, int a) -> QRgb {
        int nr = 255 - r;
        int ng = 255 - g;
        int nb = 255 - b;

        return qRgba(nr, ng, nb, a);
    });

    return image;
}

// 亮度效果(brightness)是指让图像变得更亮或更暗。算法将红色通道、绿色通道、蓝色通道，同时加上一个正值或负值。
QImage ImageEffects::brightness(QImage image, int delta) {
    map(&image, [=](int r, int g, int b, int a) -> QRgb {
        int nr = qMax(0, qMin(255, r + delta));
        int ng = qMax(0, qMin(255, g + delta));
        int nb = qMax(0, qMin(255, b + delta));

        return qRgba(nr, ng, nb, a);
    });

    return image;
}

// 把图片上每一个像素的颜色映射为函数 process() 的计算结果
void ImageEffects::map(QImage *image, std::function<QRgb (int r, int g, int b, int a)> process) {
    int w = image->width();
    int h = image->height();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            QRgb color = image->pixel(x, y); // 获取每个像素都要调用一次函数 pixel()，可以使用函数 scanLine() 提高效率

            int r = qRed(color);
            int g = qGreen(color);
            int b = qBlue(color);
            int a = qAlpha(color);

            color = process(r, g, b, a);
            image->setPixel(x, y, color);
        }
    }
}
