#include "UiUtil.h"

#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QApplication>

#include <QWidget>

void UiUtil::loadQss() {
    QStringList qssFileNames = QStringList() << "style.qss";
    QString qss;

    foreach (QString name, qssFileNames) {
        qDebug() << QString("=> Loading QSS file: %1").arg(name);

        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << QString("=> Error: Loading QSS file: %1 failed").arg(name);
            continue;
        }

        qss.append(file.readAll()).append("\n");
        file.close();
    }

    if (!qss.isEmpty()) {
        qApp->setStyleSheet(qss);
    }
}

void UiUtil::updateQss(QWidget *widget) {
    widget->setStyleSheet("/**/");
}

