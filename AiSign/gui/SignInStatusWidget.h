#ifndef SIGNINSTATUSWIDGET_H
#define SIGNINSTATUSWIDGET_H

#include <QWidget>
#include <QList>
#include <QIcon>

class Student;
class SignInInfo;
class QStandardItemModel;

namespace Ui {
class SignInStatusWidget;
}

class SignInStatusWidget : public QWidget {
    Q_OBJECT

public:
    explicit SignInStatusWidget(QWidget *parent = 0);
    ~SignInStatusWidget();

public slots:
    void setStudents(const QList<Student> &students);
    void signInSuccess(const SignInInfo &info) const; // 签到成功

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理
    Student findStudent(const SignInInfo &info) const; // 查找学生

    Ui::SignInStatusWidget *ui;
    QStandardItemModel *model;
    QIcon onlineIcon;
    QIcon offlineIcon;
    QList<Student> students;
};

#endif // SIGNINSTATUSWIDGET_H
