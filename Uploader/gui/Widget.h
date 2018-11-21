#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理
    void appendParamWidget(); // 创建参数的 widget: key input, value input, delete button, append button
    QList<QPair<QString, QString>> extractParams(); // 获取参数

    Ui::Widget *ui;
};

#endif // WIDGET_H
