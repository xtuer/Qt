#include <QApplication>
#include <QDebug>
#include <QModelIndexList>
#include "Form.h"
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include "Json.h"
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QModelIndex>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Form form;
    form.show();

    return app.exec();
}
