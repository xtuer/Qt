#include <QApplication>
#include <iostream>
#include <iomanip>
#include "ScatterMap.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ScatterMap form;
    form.show();

    return app.exec();
}
