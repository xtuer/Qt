#include "ColourfulDevicesWidget.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. 创建显示设备的 widget
    ColourfulDevicesWidget w(6);
    w.show();

    // 2. 创建设备，每个设备一个唯一的名字
    QList<QString> deviceNames;
    for (int i = 0; i < 20; ++i) {
        deviceNames << QString::number(i);
    }
    w.createDevices(deviceNames);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        // 提示: 3 和 4 同时使用
        // 3. 设置指定名字的设备的颜色
        for (int i = 0; i < deviceNames.size(); ++i) {
            // 设备的颜色可以通过 GradientColorPicker 获取
            int r = QRandomGenerator::global()->bounded(0, 256);
            int g = QRandomGenerator::global()->bounded(0, 256);
            int b = QRandomGenerator::global()->bounded(0, 256);

            w.setDeviceColor(QString::number(i), QColor(r, g, b));
        }

        // 4. 一次更新所有设备的颜色 (内部会处理设备之间的渐变色)
        w.updateDevices();
    });
    timer.start(1000);

    return a.exec();
}
