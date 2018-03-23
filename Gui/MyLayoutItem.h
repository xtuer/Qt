#ifndef MYLAYOUTITEM_H
#define MYLAYOUTITEM_H

#include <QWidgetItem>

class MyLayoutItem : public QWidgetItem {
public:
    MyLayoutItem(QWidget *widget);
    ~MyLayoutItem();
};

#endif // MYLAYOUTITEM_H
