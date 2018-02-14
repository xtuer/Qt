#include <QApplication>
#include <QDebug>
#include "ImageWidget.h"
#include <QStringList>
#include <QUrl>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageWidget w;
    w.show();

    return app.exec();
}
