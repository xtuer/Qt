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
#include "Form.h"
#include <QMap>

int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);

    QMap<QString, QString> map = {
        { "1", "One" },
        { "2", "Two" },
        { "3", "Three" },
    };

    map.insert("4", "Four");

    qDebug() << map;

//    return a.exec();
    return 0;
}
