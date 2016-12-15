#include "Bar.h"
#include <QDebug>

namespace boo {
Bar::Bar(QObject *parent) : QObject(parent) {

}

void Bar::say(int i) {
    qDebug() << "Bar::say(): " << i;
}
}
