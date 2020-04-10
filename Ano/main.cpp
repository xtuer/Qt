#include "CalloutChartWidget.h"
#include "Widget.h"

#include <QApplication>
#include <QDebug>
#include <QByteArray>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget *w = new Widget();
    w->show();

    return a.exec();
}
