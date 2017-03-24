#ifndef LOGINSTATUSWIDGET_H
#define LOGINSTATUSWIDGET_H

#include <QWidget>
#include <QList>

class Student;
class QStandardItemModel;

namespace Ui {
class LoginStatusWidget;
}

/**
 * 显示学生登陆状态的类
 */
class LoginStatusWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginStatusWidget(QWidget *parent = 0);
    ~LoginStatusWidget();

    void setStudents(const QList<Student> &students);

private:
    Ui::LoginStatusWidget *ui;
    QStandardItemModel *model;
};

#endif // LOGINSTATUSWIDGET_H
