// [1] 引入的时候有目录的加上目录 gui
#include "gui/Widget.h"
#include "bean/Foo.h"

// [2] 引入的时候没有目录的不用加
#include "Bar.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.show();

    Foo foo;
    Bar bar;

    return a.exec();
}
