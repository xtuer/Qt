#include "widget.h"

#include "HttpClient.h"

#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QNetworkAccessManager>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget w;
    w.show();

    return app.exec();
}
