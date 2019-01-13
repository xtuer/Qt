#ifndef GRADIENTPALETTE_H
#define GRADIENTPALETTE_H

#include <QLinearGradient>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

/**
 * 渐变调色板, 宽 width, 高 1px.
 * 内部颜色的存储使用 QImage.
 */
class GradientPalette {
public:
    /**
     * @brief 使用宽度创建渐变调色板
     * @param width 调色板宽度
     */
    GradientPalette(const QLinearGradient &gradient, int width);

	~GradientPalette();

    /**
     * @brief 获取指定位置处的颜色
     * @param position 范围是 [1, width]
     * @return 返回颜色
     */
    QColor getColorAt(int alpha) const;
	
private:
    void draw();    // 绘制调色板
    int width;      // 调色板的宽度
    QImage *canvas; // 作为调色板的画布
    QLinearGradient gradient; // 线性渐变
};

#endif /* GRADIENTPALETTE_H */
