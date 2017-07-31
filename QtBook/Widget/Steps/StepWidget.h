#ifndef STEPWIDGET_H
#define STEPWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QList>

class StepWidget : public QWidget {
    Q_OBJECT

public:
    StepWidget(QWidget *parent = 0);
    ~StepWidget();

protected slots:
    // 步骤按钮的槽，把被点击的按钮切换为当前步骤的按钮，然后更新样式
    void updateStepButtonsStyle();

private:
    void createLoadQssAction();
    QList<QPushButton*> stepButtons;
};

#endif // STEPWIDGET_H
