#ifndef COLOURFULDEVICESWIDGET_H
#define COLOURFULDEVICESWIDGET_H

#include <QWidget>
#include <QList>

class ColourfulDeviceWidget;

namespace Ui {
class ColourfulDevicesWidget;
}

/**
 * 显示设备的 widget
 */
class ColourfulDevicesWidget : public QWidget {
    Q_OBJECT

public:
    // 创建 ColourfulDevicesWidget 对象，指定每列显示的设备数
    explicit ColourfulDevicesWidget(int columnCountOfDevice, QWidget *parent = nullptr);
    ~ColourfulDevicesWidget();

    // 使用设备的名字初始化设备列表
    void createDevices(QList<QString> deviceNames);

    // 设置名字为 deviceName 的设备的颜色
    void setDeviceColor(const QString &deviceName, const QColor &color);

    // 一次更新所有设备的颜色 (内部会处理设备之间的渐变色)
    void updateDevices();

private:
    void initialize();
    void handleEvents();
    void placeDevices(); // 放置设备
    void emptyWidget(QWidget *widget); // 删除 widget 里面的所有子 widget 和 layout

    Ui::ColourfulDevicesWidget *ui;
    QList<ColourfulDeviceWidget *> devices;
    int columnCountOfDevice; // 每列设备数
};

#endif // COLOURFULDEVICESWIDGET_H
