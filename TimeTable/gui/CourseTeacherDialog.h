#ifndef COURSETEACHERDIALOG_H
#define COURSETEACHERDIALOG_H

#include <QDialog>

namespace Ui {
class CourseTeacherDialog;
}

class CourseTeacherDialog : public QDialog {
    Q_OBJECT

public:
    explicit CourseTeacherDialog(QWidget *parent = 0);
    ~CourseTeacherDialog();

private:
    Ui::CourseTeacherDialog *ui;
};

#endif // COURSETEACHERDIALOG_H
