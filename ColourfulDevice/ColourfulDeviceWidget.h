#ifndef COLOURFULDEVICEWIDGET_H
#define COLOURFULDEVICEWIDGET_H

#include <QWidget>

/**
 * 设备 widget，每个设备都有名字，并设置为设备 widget 的 objectName，所以名字最好是唯一的，这样可以通过名字查找设备
 */
class ColourfulDeviceWidget : public QWidget {
    Q_OBJECT

public:
    ColourfulDeviceWidget(const QString &name, QWidget *parent = nullptr);

    // 获取设备的名字
    QString getName() const;

    // 设置设备的颜色
    void setColor(const QColor &color);

    // 获取设备的颜色
    QColor getColor() const;

    // 设置渐变开始和结束的颜色
    void setGradientColors(const QColor &startColor, const QColor &endColor);

protected:
    void paintEvent(QPaintEvent *event) override;

    // 设备的名字，最好是唯一的
    QString name;

    // 渐变开始、中心、结束的颜色
    QColor startColor  = Qt::transparent;
    QColor centerColor = Qt::transparent;
    QColor endColor    = Qt::transparent;
};

#endif // COLOURFULDEVICEWIDGET_H
