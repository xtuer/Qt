#include <QApplication>
#include <QDebug>
#include "CertDegisner.h"
#include "MyWidget.h"
#include <QGridLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

//    MyWidget form;
//    form.show();
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QWidget());
    layout->addWidget(new QWidget());

    QLayoutItem *child;

    while ((child = layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    unsigned char b = 150;
    qDebug() << b;

    return app.exec();
}
