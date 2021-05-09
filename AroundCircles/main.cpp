#include <QApplication>
#include <QDebug>
#include "ArrangeDevicesWidget.h"
#include "AroundDevicesGraphicsView.h"

#include <QList>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    // [1] 设备名字，每个设备的名字唯一
    QStringList deviceNames;
    for (int i = 1; i <= 16; ++i) {
        deviceNames << QString("Device-%1").arg(i);
    }

    // [2] 布点模式一: 圆环布点占位
//    ArrangeDevicesWidget w1(1, deviceNames);
//    w1.show();

    // [3] 布点模式二: 放下探头生成布点
//    ArrangeDevicesWidget w2(2, deviceNames);
//    w2.show();

    // [3] 布点模式三: 指定行列布点占位
//    ArrangeDevicesWidget w3(3, deviceNames);
//    w3.initMode3(8, 5);
//    w3.show();

    // [4] 布点模式四: 16 个布点占位
    ArrangeDevicesWidget w4(4, deviceNames);
    w4.show();

    return app.exec();
}


