#include <QApplication>
#include <QLabel>
#include <QPoint>
#include <QRandomGenerator>

#include "heatmap/Heatmap.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    int w = 600;
    int h = 400;
    int max = 100;

    // [1] 使用热力图的宽高和要显示的最大值创建热力图
    Heatmap heatmap(w, h, max);

    // [2] 随机生成点和点对应的温度添加到热力图
    for (int i = 0; i < 50; ++i) {
        int x = QRandomGenerator::global()->bounded(w);
        int y = QRandomGenerator::global()->bounded(h);
        qreal value = QRandomGenerator::global()->bounded(max);
        heatmap.addPoint(x, y, value);
    }

    QLabel label;
    label.setPixmap(heatmap.getHeatmap()); // [3] 显示热力图
    label.show();

    return a.exec();
}
