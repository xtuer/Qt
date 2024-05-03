#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QButtonGroup;
class QAbstractButton;

namespace Ui {
class CentralWidget;
}

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    /**
     * @brief 设置 slot 按钮的状态，目的是为你更新按钮的样式。
     * @param buttonObjectName 按钮的名称。
     * @param stateValue 状态值，例如 red, green, yellow。
     */
    void setSlotButtonState(const QString buttonObjectName, const QString stateValue);

private:
    void initUi();
    void handleEvents();
    void createWidgetInContentStackedWidget(QAbstractButton *slotButton); // 创建 slot 按钮对应的 widget。

    Ui::CentralWidget *ui;
    QButtonGroup *buttonGroup; // 按钮组，slot 按钮为一个按钮组。
    QHash<QAbstractButton *, QWidget *> buttonWidgetHash; // key 是 slot 按钮的指针，value 是右侧 slot widget 的指针。
};

#endif // CENTRALWIDGET_H
