#include "Calculator.h"
#include <QDebug>

Calculator::Calculator() {

}

int Calculator::add(int a, int b) const {
    return a + b;
}

void doWork() {
    qDebug() << "work() -> doWork()";
}

void work() {
    doWork();
}
