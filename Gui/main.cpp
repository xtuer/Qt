#include "widget.h"

#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QDateTime>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget *w = new Widget();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    return app.exec();
}
