#include <QApplication>
#include <QDebug>
#include <QModelIndexList>
#include "Form.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Form form;
    form.show();

    app.setStyleSheet("#pushButton { background: blue; color: white; }");

    return app.exec();
}
