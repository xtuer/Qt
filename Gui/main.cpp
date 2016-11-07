#include "widget.h"

#include <QDebug>
#include <QApplication>
#include <QXmlQuery>
#include "JsonReader.h"
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget *w = new Widget();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    return app.exec();
}
