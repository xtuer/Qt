#ifndef CLASSCOURSETEACHERWIDGET_H
#define CLASSCOURSETEACHERWIDGET_H

#include <QWidget>

namespace Ui {
class ClassCourseTeacherWidget;
}

class ClassCourseTeacherWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClassCourseTeacherWidget(QWidget *parent = 0);
    ~ClassCourseTeacherWidget();

private:
    Ui::ClassCourseTeacherWidget *ui;
};

#endif // CLASSCOURSETEACHERWIDGET_H
