#include "ColourfulDevicePlazza.h"
#include <QApplication>
#include <QRandomGenerator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ColourfulDevicePlazza w;
    w.show();

    // 设置左下角的坐标信息
    w.setPositiveCoordinates("H1 A1");
    w.setNegativeCoordinates("D10 E12");

    // 设置中间设备对应圆的背景
    int rows = 8;
    int cols = 12;
    for (int row = 1; row <= rows; ++row) {
        for (int col = 1; col <= cols; ++col) {
            // 提示: 设备的名字即是它的坐标，如 A1, D23, E3
            QString name = QString("%1%2").arg(char('A'+row-1)).arg(col);

            int r = QRandomGenerator::global()->bounded(0, 256);
            int g = QRandomGenerator::global()->bounded(0, 256);
            int b = QRandomGenerator::global()->bounded(0, 256);
            bool stripe = QRandomGenerator::global()->generate() % 5 == 0;

            w.setDeviceBackground(name, QColor(r, g, b), stripe);
        }
    }

    return a.exec();
}
