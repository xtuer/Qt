#include "ColourfulDeviceWidget.h"

#include <QDebug>
#include <QVariant>
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>

ColourfulDeviceWidget::ColourfulDeviceWidget(const QString &name, QWidget *parent) : QWidget(parent), name(name) {
    this->setObjectName(name);
    this->setProperty("class", QString("ColourfulDeviceWidget"));
    this->setMinimumSize(100, 65);
    this->setAttribute(Qt::WA_StyledBackground, true);
}

ColourfulDeviceWidget::~ColourfulDeviceWidget() {
}

// 获取设备的名字
QString ColourfulDeviceWidget::getName() const {
    return name;
}

// 设置设备的颜色
void ColourfulDeviceWidget::setColor(const QColor &color) {
    this->centerColor = color;
}

// 获取设备的颜色
QColor ColourfulDeviceWidget::getColor() const {
    return centerColor;
}

// 设置渐变开始、中心、结束的颜色
void ColourfulDeviceWidget::setGradientColors(const QColor &startColor, const QColor &endColor) {
    this->startColor = startColor;
    this->endColor   = endColor;
}

void ColourfulDeviceWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int p = 4; // padding

    QLinearGradient gradient(-w/2.0, 0.0, w*3/2.0, 0.0);
    gradient.setColorAt(0.0, startColor);
    gradient.setColorAt(0.45, centerColor);
    gradient.setColorAt(0.55, centerColor);
    gradient.setColorAt(1.0, endColor);

    painter.setPen(QPen(centerColor, 2));
    painter.setBrush(gradient);
    painter.drawRoundedRect(p, p, w-p-p, h-p-p, 4, 4);

    painter.setPen(Qt::black);
    painter.drawText(0, 0, w, h, Qt::AlignCenter, name);
}
