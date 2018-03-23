#include "MyLayoutItem.h"
#include <QDebug>

MyLayoutItem::MyLayoutItem(QWidget *widget) : QWidgetItem(widget) {

}

MyLayoutItem::~MyLayoutItem() {
    qDebug() << "~MyLayoutItem()";
}
