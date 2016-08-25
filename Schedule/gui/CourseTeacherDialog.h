#ifndef COURSETEACHERDIALOG_H
#define COURSETEACHERDIALOG_H

#include <QDialog>

namespace Ui {
class CourseTeacherDialog;
}

class CourseTeacherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CourseTeacherDialog(QWidget *parent = 0);
    ~CourseTeacherDialog();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    void loadCourses();  // 加载课程
    void loadTeachers(); // 加载老师

    Ui::CourseTeacherDialog *ui;
};

#endif // COURSETEACHERDIALOG_H
