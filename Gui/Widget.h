#pragma once
#include <QWidget>

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
};
