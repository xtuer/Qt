#ifndef TEACHERSCHEDULEWIDGET_H
#define TEACHERSCHEDULEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QSqlTableModel;
class QShowEvent;
class ScheduleItem;

namespace Ui {
class TeacherScheduleWidget;
}

class TeacherScheduleWidget : public QWidget {
    Q_OBJECT

public:
    explicit TeacherScheduleWidget(QWidget *parent = 0);
    ~TeacherScheduleWidget();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeTeacherModel();
    void showSchedule(int teacherId); // 显示老师的课表
    void handleEvents();

    Ui::TeacherScheduleWidget *ui;
    QSqlTableModel *teacherModel;
    QSqlDatabase db;
    int currentTeacherId; // 当前班级的 id
};

#endif // TEACHERSCHEDULEWIDGET_H
