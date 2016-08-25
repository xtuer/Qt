#ifndef MANAGECLAZZCOURSEWIDGET_H
#define MANAGECLAZZCOURSEWIDGET_H

#include <QWidget>

namespace Ui {
class ManageClazzCourseTeacherWidget;
}

class ManageClazzCourseTeacherWidget : public QWidget {
    Q_OBJECT

public:
    explicit ManageClazzCourseTeacherWidget(QWidget *parent = 0);
    ~ManageClazzCourseTeacherWidget();

private:
    Ui::ManageClazzCourseTeacherWidget *ui;
};

#endif // MANAGECLAZZCOURSEWIDGET_H
