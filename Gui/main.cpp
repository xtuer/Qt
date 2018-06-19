#include <QApplication>
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "ImageWidget.h"
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);

//    ImageWidget w;
//    w.show();

//    return a.exec();
    double x = 134.97232;

    for (int i = 1; i < 7; ++i) {
        qDebug() << QString("%1").arg(x, 0, 'f', i, '0');
    }


    return 0;
}
