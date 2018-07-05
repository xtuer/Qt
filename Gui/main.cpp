#include <QApplication>
#include <QDebug>
#include <QModelIndexList>
#include "Form.h"
#include <QDir>
#include <QFile>

int main(int argc, char *argv[]) {
    qDebug() << QFile::symLinkTarget("justica");
    qDebug() << QFile::symLinkTarget("foo");

    return 0;
}
