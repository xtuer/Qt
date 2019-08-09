#ifndef COLOURFULDEVICEPLAZZA_H
#define COLOURFULDEVICEPLAZZA_H

#include <QWidget>

namespace Ui {
class ColourfulDevicePlazza;
}

class ColourfulDevicePlazza : public QWidget {
    Q_OBJECT

public:
    explicit ColourfulDevicePlazza(QWidget *parent = nullptr);
    ~ColourfulDevicePlazza();

    /**
     * 使用名字为传入的 deviceName 的设置设备的背景
     *
     * @param deviceName 设备名字
     * @param bgcolor    背景色名字
     * @param stripe     是否条形背景
     */
    void setDeviceBackground(const QString &deviceName, const QColor &bgcolor, bool stripe);

    /**
     * 设置左下角 positive 的坐标
     *
     * @param text 坐标的文本
     */
    void setPositiveCoordinates(const QString &text);

    /**
     * 设置左下角 negative 的坐标
     *
     * @param text 坐标的文本
     */
    void setNegativeCoordinates(const QString &text);

private:
    void initialize();    // 初始化
    void createDevices(); // 创建设备

    Ui::ColourfulDevicePlazza *ui;
};

#endif // COLOURFULDEVICEPLAZZA_H
