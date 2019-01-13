#ifndef HEATMAP_H
#define HEATMAP_H

#include "global.h"
#include <QVector>

class QImage;
class QPixmap;
class QLinearGradient;
class GradientPalette;

/**
 * @brief 热力图
 * 原理: https://blog.csdn.net/qinpanke/article/details/21977279
 *
 * 使用方法:
 * 1. 创建热力图对象: Heatmap heatmap(w, h, max);
 * 2. 给热力图增加要显示的点: heatmap.addPoint(150, 110, 67);
 * 3. 获取热力图的图片: heatmap.getHeatmap();
 */
class QHEATMAP_DLL_EXPORT Heatmap {
public:
    /**
     * @brief 创建热力图对象
     *
     * @param width   热力图的宽
     * @param height  热力图的高
     * @param max     热力图表示的值的最大值, 例如要画的热力图中, 最高温度设定为 100 度
     * @param radius  半径，决定了径向渐变的大小
     * @param opacity 透明度, 范围为 [0, 255]
     */
    Heatmap(int width, int height, qreal max, int radius = 60, int opacity = 168);
    virtual ~Heatmap();

    /**
     * @brief 热力图上增加点
     *
     * @param x 横坐标, 范围为 [0, width)
     * @param y 纵坐标, 范围为 [0, height)
     * @param value 点上对应的值
     */
    void addPoint(int x, int y, qreal value);

    /**
     * @brief 获取热力图的图片
     * @return 返回热力图的 pixmap
     */
    QPixmap getHeatmap();

    /**
     * @brief 保存热力图
     * @param path 图片保存路径
     */
    void save(const QString &path);

    /**
     * @brief 默认已经提供了一个线性渐变, 可以调用这个函数进行修改
     * @param gradient 线性渐变
     */
    void setGradient(const QLinearGradient &gradient);

protected:
    virtual void drawAlpha(int x, int y, qreal value);
    virtual void colorize(int left, int top, int right, int bottom);
	
private:
    void draw(); // 绘制热力图

    QVector<qreal> data; // 存储每个点的值，大小和图像一样
    QImage *alphaCanvas = nullptr; // 用于存储渐变透明数据的黑白图像
    QImage *mainCanvas  = nullptr; // 用于显示输出的图像
    GradientPalette *palette = nullptr; // 调色板

    int radius;    // 半径
    int opacity;   // 不透明度
    int width;     // 图像宽度
    int height;    // 图像高度
    qreal max = 0; // 最大值
};

#endif /* HEATMAP_H */


















