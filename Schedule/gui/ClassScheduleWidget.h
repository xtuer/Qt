#ifndef CLASSSCHEDULEWIDGET_H
#define CLASSSCHEDULEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QSqlTableModel;
class QShowEvent;
class ScheduleItem;

class CourseTeacherDialog;

namespace Ui {
class ClassScheduleWidget;
}

/**
 * @brief 班级排课的类
 */
class ClassScheduleWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClassScheduleWidget(QWidget *parent = 0);
    ~ClassScheduleWidget();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeClassModel();
    void handleEvents();
    void showSchedule(int classId); // 显示班级的课表
    void showArrangedSchedule(int classId); // 使用预先分配的课程排课
    // 取得当前课表的 item, valid 为 true 时表示只选择 courseId 或 teacherId 不为 0 的 item
    // 因为默认每个 schedule item 都是有 classId 的，所以不能使用它来判断
    QList<ScheduleItem> getScheduleItems(bool valid = false) const;
    void shuffleSchedule(QList<ScheduleItem> *items); // 随机排课

    Ui::ClassScheduleWidget *ui;
    QSqlTableModel *classModel;
    QSqlDatabase db;
    int currentClassId; // 当前班级的 id
    CourseTeacherDialog *courseTeacherDialog;
};

#endif // CLASSSCHEDULEWIDGET_H
