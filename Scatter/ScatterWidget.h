#ifndef SCATTERWIDGET_H
#define SCATTERWIDGET_H

#include <QWidget>

class ScatterMap;

namespace Ui {
class ScatterWidget;
}

class ScatterWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScatterWidget(QWidget *parent = nullptr);
    ~ScatterWidget();

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理

    Ui::ScatterWidget *ui;
    ScatterMap *scatterMap = nullptr;
};

#endif // SCATTERWIDGET_H
