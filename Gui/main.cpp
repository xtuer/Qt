#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include <QMap>

#include "Json.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QList<QMap<int, QString>> list;
    list << (QMap<int, QString>{{1, "One"}, {2, "Two"}});
    return 0;
}
