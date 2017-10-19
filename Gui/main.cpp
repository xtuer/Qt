#include <QApplication>
#include <QDebug>
#include "Panel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Panel panel;
    panel.resize(700, 400);
    panel.show();

    return app.exec();
}
