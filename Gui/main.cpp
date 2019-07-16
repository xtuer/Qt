#include <QApplication>
#include <QDebug>

#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include "Widget.h"
#include "AnimatedCheckBox.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget *w = new Widget();
    AnimatedCheckBox *checkbox = new AnimatedCheckBox();
    QPushButton *button = new QPushButton("Click");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(checkbox);
//    layout->addWidget(button);
    w->setLayout(layout);
    w->resize(160, 80);
    w->show();

    return app.exec();
}
