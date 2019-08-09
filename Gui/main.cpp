#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>
#include "Thread.h"
#include "MyWidget.h"
#include <QRandomGenerator>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MyWidget w;
    w.show();

    return app.exec();
}
