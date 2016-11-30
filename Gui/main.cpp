//#include <QApplication>
//#include "widget.h"
#include <QDebug>
#include <QObject>

class Foo {
public:
    operator QVariant() const {
        return QVariant::fromValue(*this);
    }

    int amount;
};

Q_DECLARE_METATYPE(Foo)

///////////////////////////////////////////////////////////
///                         main()                       //
///////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qRegisterMetaType<Foo>();
    // connect...

    return 0;
}

//class Foo {
//public:
//    int value;

//    operator QVariant() const {
//        return QVariant::fromValue(*this);
//    }
//};

//Q_DECLARE_METATYPE(Foo)

//int main(int argc, char *argv[]) {
//    QObject obj;
//    Foo foo;
//    foo.value = 12;

//    obj.setProperty("foo", foo);
//    qDebug() << obj.property("foo").value<Foo>().value;

//    return 0;
//}
