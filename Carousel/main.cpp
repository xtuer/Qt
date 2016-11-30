#include "Carousel.h"
#include <QApplication>
#include <QList>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QList<QString> imagePaths;

    // 初始化 items
    for (int i = 1; i <= 17; ++i) {
        imagePaths << QString("images/%1.jpg").arg(i);
    }

    Carousel w(200, 124, 0.4, 300, imagePaths);
    w.resize(800, 300);
    w.show();

    return a.exec();
}
