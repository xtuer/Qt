#include "PaintedBorderImageWidget.h"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget *window = new QWidget();
    window->setWindowTitle("Painted Border Image");
    window->resize(340, 300);

    PaintedBorderImageWidget *w1 = new PaintedBorderImageWidget(window);
    w1->setGeometry(20, 20, 300, 50);

    PaintedBorderImageWidget *w2 = new PaintedBorderImageWidget(window);
    w2->setGeometry(20, 80, 100, 120);

    PaintedBorderImageWidget *w3 = new PaintedBorderImageWidget(window);
    w3->setGeometry(20, 210, 80, 25);

    PaintedBorderImageWidget *w4 = new PaintedBorderImageWidget(window);
    w4->setGeometry(130, 80, 190, 200);

    window->show();

    return a.exec();
}
