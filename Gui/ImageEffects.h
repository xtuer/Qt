#ifndef IMAGEEFFECTS_H
#define IMAGEEFFECTS_H
#include <QImage>

class ImageEffects {
public:
    /**
     * 灰度效果
     * 灰度效果(grayscale)就是取红、绿、蓝三个像素值的算术平均值，这实际上将图像转成了黑白形式。
     *
     * @param image 原始图片
     * @return 返回新的效果图
     */
    static QImage gray(QImage image);

    /**
     * 复古效果
     * 复古效果(sepia)则是将红、绿、蓝三个像素，分别取这三个值的某种加权平均值，使得图像有一种古旧的效果。
     *
     * @param image 原始图片
     * @return 返回新的效果图
     */
    static QImage siepa(QImage image);

    /**
     * 红色蒙版
     * 红色蒙版指的是，让图像呈现一种偏红的效果。算法是将红色通道设为红、绿、蓝三个值的平均值，而将绿色通道和蓝色通道都设为 0。
     *
     * @param image 原始图片
     * @return 返回新的效果图
     */
    static QImage red(QImage image);

    /**
     * 反转效果
     * 反转效果(invert)是指图片呈现一种色彩颠倒的效果。算法为红、绿、蓝通道都取各自的相反值(255-原值)。
     *
     * @param image 原始图片
     * @return 返回新的效果图
     */
    static QImage invert(QImage image);

    /**
     * 亮度效果
     * 亮度效果(brightness)是指让图像变得更亮或更暗。算法将红色通道、绿色通道、蓝色通道，同时加上一个正值或负值。
     *
     * @param image 原始图片
     * @return 返回新的效果图
     */
    static QImage brightness(QImage image, int delta);

    /**
     * 把图片上每一个像素的颜色映射为函数 process() 的计算结果。
     *
     * @param image   要进行变换的图片指针
     * @param process 对每一个像素的颜色进行计算的函数
     */
    static void map(QImage *image, std::function<QRgb (int r, int g, int b, int a)> process);
};

#endif // IMAGEEFFECTS_H
