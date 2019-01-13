#include "GradientPalette.h"

#include <QImage>
#include <QPainter>
#include <QDebug>

// 使用宽度创建渐变调色板
GradientPalette::GradientPalette(const QLinearGradient &gradient, int width) {
    this->width    = width;
    this->canvas   = new QImage(width, 1, QImage::Format_ARGB32);
    this->gradient = QLinearGradient(0, 0, width, 0);
    this->gradient.setStops(gradient.stops());

    draw();
}

GradientPalette::~GradientPalette() {
    delete canvas;
}

// 获取指定位置处的颜色
QColor GradientPalette::getColorAt(int alpha) const {
    int position = static_cast<int>(alpha / 255.0 * width) - 1;

    if (position > width) {
		return Qt::color0;
    }

    return canvas->pixel(static_cast<int>(position) , 0);
}

void GradientPalette::draw() {
    QPainter painter(canvas);
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.fillRect(canvas->rect(), gradient);
}
