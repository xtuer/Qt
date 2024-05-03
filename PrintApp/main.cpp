#include "CentralWidget.h"

#include <QApplication>
#include <QShortcut>
#include <QFile>
#include <QDebug>

void loadQss() {
    qDebug() << "Load QSS";
    QFile file("/Users/biao/Desktop/style.qss"); // QSS 文件路径
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString qss(file.readAll());
    qApp->setStyleSheet(qss);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CentralWidget w;
    w.show();

    loadQss();

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_L), &w);
    QObject::connect(shortcut, &QShortcut::activated, [] {
        loadQss();
    });

    return app.exec();
}
