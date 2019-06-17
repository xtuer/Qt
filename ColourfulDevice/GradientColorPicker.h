#ifndef GRADIENTCOLORPICKER_H
#define GRADIENTCOLORPICKER_H

#include <QColor>

class QLinearGradient;
class GradientColorPickerPrivate;

/**
 * @brief 线性渐变的颜色拾取器
 *
 * 使用方法:
 * 1. 创建拾取器: GradientColorPicker picker;
 * 2. 获取颜色: QColor color = picker.getColor(0.50);
 */
class GradientColorPicker {
public:
    // 使用默认的线性渐变颜色
    GradientColorPicker();

    // 使用自定义的线性渐变颜色
    GradientColorPicker(const QLinearGradient &gradient);

    // 析构函数
    ~GradientColorPicker();

    /**
     * @brief 获取 value 对应的颜色值，value 的范围为 [0, 1]
     *
     * @param value 数值
     * @return 返回数值对应的颜色
     */
    QColor getColor(double value) const;

private:
    GradientColorPickerPrivate *d;
};

#endif // GRADIENTCOLORPICKER_H
