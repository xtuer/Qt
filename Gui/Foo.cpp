#include "Foo.h"
#include <QDebug>

namespace xtuer {

Foo::Foo(QObject *parent) : QObject(parent) {

}

void Foo::say() {
    emit hello(23);
}

void Foo::bar() {
    qDebug() << "Foo::bar()";
}
}
