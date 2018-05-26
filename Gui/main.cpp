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
#include "MyWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MyWidget w;
    w.show();

    return a.exec();
}
