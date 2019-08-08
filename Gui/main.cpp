#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>
#include "Thread.h"

class A {
public:
    int a = 2;
};

class B : virtual public A {

};

class C : virtual public A {

};

class D : public B, public C {

};


int main(int argc, char** argv) {
    D d;

    qDebug() << d.a;

    return 0;
}
