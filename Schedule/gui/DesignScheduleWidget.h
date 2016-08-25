#ifndef DESIGNSCHEDULEWIDGET_H
#define DESIGNSCHEDULEWIDGET_H

#include <QWidget>

class ScheduleDescription;

namespace Ui {
class DesignScheduleWidget;
}

/**
 * @brief 设计课表
 * 双击星期或者课程时间的 Label 调用 editDayOrTimeText() 进行编辑他们
 * 改变 spin box 的值可以修改每周上课的天数和每天的课程数，在 handleEvents() 里绑定事件处理函数
 * 构造函数中调用 readScheduleDescription() 从文件读取课程表结构描述的数据，用于创建课程表
 * 点击保存按钮调用 writeScheduleDescription() 保存课程表结构描述的数据到文件
 */
class DesignScheduleWidget : public QWidget {
    Q_OBJECT

public:
    explicit DesignScheduleWidget(QWidget *parent = 0);
    ~DesignScheduleWidget();

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    void handleEvents(); // 事件处理
    void showSchedule(); // 显示课程表
    void editDayOrTimeText(int x, int y); // 编辑星期和课程时间

    Ui::DesignScheduleWidget *ui;
    ScheduleDescription *scheduleDescription; // 课程表结构描述数据
};

#endif // DESIGNSCHEDULEWIDGET_H
