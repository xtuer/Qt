#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>
#include "Thread.h"

class BaseA {
public:
    void update() {
        qDebug() << "BaseA::update()";
    }
};

class BaseB {
public:
    virtual ~BaseB();

     void update() {
        qDebug() << "BaseB::update()";
    }
};

BaseB::~BaseB() {}

class Child : public BaseA, public BaseB {
public:
    ~Child() override;

    void update() {
        qDebug() << "Child::update()";
        BaseA::update();
    }
};

Child::~Child() {}

int main(int argc, char *argv[]) {
    BaseA *child1 = new Child();
    child1->update();

    BaseB *child2 = new Child();
    child2->update();

//    QApplication app(argc, argv);

//    Thread thread;
//    thread.start();

//    qDebug() << "Main: " << QThread::currentThread();

//    return app.exec();
}
