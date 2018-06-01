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
#include "ScatterMap.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    ScatterMap *scatterMap = new ScatterMap();
    QLayout *layout = new QVBoxLayout();
    layout->addWidget(scatterMap);

    QWidget window;
    window.setLayout(layout);
    window.resize(500, 400);
    window.setStyleSheet(".Scatter { background: #2b85e4; border-radius: 10px; min-width: 20px; max-width: 20px; min-height: 20px; max-height: 20px; }");
    window.show();

    return a.exec();
}
