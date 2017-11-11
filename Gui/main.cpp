#include <QApplication>
#include <QDebug>
#include "Widget.h"

#include <QSettings>
#include <QTextCodec>
#include <QThreadPool>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << QThreadPool::globalInstance()->maxThreadCount();

    return app.exec();
}
