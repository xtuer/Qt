#ifndef SCATTERHOLDER_H
#define SCATTERHOLDER_H

#include <QWidget>

class ScatterMap;

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理

    Ui::Widget *ui;
    ScatterMap *scatterMap = 0;
};

#endif // SCATTERHOLDER_H
