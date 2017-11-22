#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class Site;
class Person;
class Student;
class MainWidgetPrivate;

namespace Ui {
class MainWidget;
}

/**
 * 主要逻辑与调用说明:
 *
 * 点击 “初始化” 按钮调用 loadRoomInformation() 加载考试的初始化信息，然后选择 “考期” “考点”，
 * 选择 “考场” 的时候调用 loadRoomEnrollment() 加载此考点学生的刷卡签到数据
 * 点击 “详情” 调用 xxx() 可视化显示此考点学生的刷卡签到数据
 * 点击 “开始读卡” 连接身份证读卡器，启动线程 ReadThread 循环的读取身份证信息
 * ReadThread 读取到身份证信息后发出信号 personReady()，然后调用 signIn() 发送学生身份证信息到服务器签到
 * 签到成功则 “已刷卡” 数加 1，“未刷卡” 数减 1
 *
 * 调用 showInfo() 显示信息到界面上
 * 信号槽连接放在 handleEvents() 里创建
 */
class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void personReady(const Person &p); // 得到学生的信息
    void login(const Person &p); // 发送学生信息到服务器签到
    void showInfo(const QString &info, bool error = false); // 显示信息, error 为 true 时以红色显示
    void setReadButtonText(const QString &text);

private:
    void handleEvents(); // 事件处理
    void loadStudents();  // 加载学生数据
    void loadSiteAndPeriodUnit(); // 加载考期和考点
    void loadRoom(const QString &siteCode); // 加载考场

    void loginSuccess(const QString &idCardNo); // idCardNo 的学生登陆成功
    void updateLoginStatistics(const QList<Student> &students); // 更新刷卡统计

    void mocLoadStudents();
    void showLoginStatusWidget(const QList<Student> &students); // 可视化的显示登陆状态
    void showPerson(const Person &p);
    QString uploadCameraPictureName(); // 上传的考生照片的名字

    friend class MainWidgetPrivate;

    Ui::MainWidget *ui;
    MainWidgetPrivate *d;
};

#endif // MAINWIDGET_H
