#include <QApplication>
#include <QDebug>
#include "NewForm.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    NewForm form;
    form.show();

    return app.exec();
}
