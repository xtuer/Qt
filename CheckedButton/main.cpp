#include "Widget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    a.setStyleSheet(R"(
        .CheckedButton {
            border-radius: 4px;
        }

        .CheckedButton[checked=true] {
            background: blue;
        }

        .CheckedButton[checked=false] {
            background: red;
        }

QAbstractSpinBox {
    height: 24px;
    min-width: 60px;
    font-size: 12px;
    border: 1px solid #dddee1;
    border-radius: 4px;
    color: #495060;
    background-color: #fff;
    padding: 0 1px;
}

QAbstractSpinBox:focus {
    border-color: #57a3f3;
}

QAbstractSpinBox::down-button, QAbstractSpinBox::up-button {
    subcontrol-origin: border;
    width: 16px;
    height: 16px;
    border-width: 0 0 0 0;
}
QAbstractSpinBox::down-button {
    subcontrol-position: center left;
    margin-left: 5px;
    border-image: url(/Users/biao/Desktop/left.png) 0 0 0 0 stretch stretch;
}
QAbstractSpinBox::up-button {
    subcontrol-position: center right;
    margin-right: 5px;
    border-image: url(img/common/plus.png) 0 0 0 0 stretch stretch;
}
    )");

    Widget w;
    w.show();
    return a.exec();
}
