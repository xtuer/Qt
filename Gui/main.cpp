#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include "Thread.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton b("Button");
    b.show();

    QObject::connect(&b, &QPushButton::clicked, [&] {
        b.hide();
    });

    return app.exec();
}
