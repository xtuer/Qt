#ifndef COURSETEACHERWIDGET_H
#define COURSETEACHERWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QLabel>

namespace Ui {
class CourseTeacherWidget;
}

class CourseTeacherWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseTeacherWidget(QWidget *parent = 0);
    ~CourseTeacherWidget();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void showEvent(QShowEvent *event);

private:
    void loadCourses();
    void loadTeachers();
    void startDrag(QLabel *label);

    Ui::CourseTeacherWidget *ui;
    QPoint startPos;
};

#endif // COURSETEACHERWIDGET_H
