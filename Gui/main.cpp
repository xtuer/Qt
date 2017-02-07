#include <QApplication>
#include <QDebug>
#include <QList>
#include "Widget.h"
#include <QPushButton>
#include <QShortcut>
#include <QKeySequence>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RoundButtonWidget w;
    w.show();

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), &w);

    QObject::connect(shortcut, &QShortcut::activated, [] {
        qDebug() << "hi";
    });

    return app.exec();
}
