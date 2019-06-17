#include "GradientColorPicker.h"

#include <QPainter>
#include <QImage>
#include <QLinearGradient>

class GradientColorPickerPrivate {
    friend class GradientColorPicker;

    // 使用代理构造函数创建对象
    GradientColorPickerPrivate() : GradientColorPickerPrivate(defaultGradient()) {}

    GradientColorPickerPrivate(const QLinearGradient &gradient) {
        // 图片的高宽
        int w = 1001;
        int h = 3;

        // 设置渐变的范围
        QLinearGradient g(gradient);
        g.setStart(0, 0);
        g.setFinalStop(w, 0);

        // 在 image 中绘制渐变的矩形
        image = QImage(w, h, QImage::Format_ARGB32);
        QPainter painter(&image);
        painter.setPen(Qt::NoPen);
        painter.setBrush(g);
        painter.drawRect(0, 0, w, h);
        painter.end();

        image.save("gradient-color-picker.png");
    }

    // 获取 value 对应的颜色值，value 的范围为 [0, 1]
    QColor getColor(double value) const {
        int x = int(clamp(0, 1, value) * (image.width()-1));

        return image.pixelColor(x, 2);
    }

    // 限制 value 在 [start, end] 的范围中
    qreal clamp(qreal start, qreal end, qreal value) const {
        value = qMax(start, value);
        value = qMin(value, end);

        return value;
    }

    // 创建默认的颜色线性渐变
    static QLinearGradient defaultGradient() {
        QLinearGradient gradient;
        gradient.setColorAt(0.0, Qt::white);
        gradient.setColorAt(0.3, Qt::yellow);
        gradient.setColorAt(1.0, Qt::red);

        return gradient;
    }

    QImage image;
};

GradientColorPicker::GradientColorPicker() : GradientColorPicker(GradientColorPickerPrivate::defaultGradient()) {

}

GradientColorPicker::GradientColorPicker(const QLinearGradient &gradient) {
    d = new GradientColorPickerPrivate(gradient);
}

GradientColorPicker::~GradientColorPicker() {
    delete d;
}

QColor GradientColorPicker::getColor(double value) const {
    return d->getColor(value);
}
