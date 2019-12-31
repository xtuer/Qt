#ifndef AISIGNWIDGET_H
#define AISIGNWIDGET_H

#include <QWidget>
#include <QList>

class Person;
class Student;
class SignInInfo;

struct AiSignWidgetPrivate;

namespace Ui {
class AiSignWidget;
}

class AiSignWidget : public QWidget {
    Q_OBJECT

public:
    explicit AiSignWidget(QWidget *parent = nullptr);
    ~AiSignWidget();

public slots:
    void showInfo(const QString &info, bool ok = true) const; // 显示信息, ok 为 false 时以红色显示
    void personReady(const Person &p);                   // 身份证信息读取成功
    void updateSystemStatus(QWidget *w, bool ok = true); // 更新系统状态
    void signInSuccess(const SignInInfo &info) const;    // 签到成功

signals:
    void studentsReady(const QList<Student> &students);

private:
    void initialize();        // 初始化
    void handleEvents();      // 信号槽事件处理
    void startIdCardReader(); // 启动身份证刷卡器
    void startCamera();       // 启动摄像头

    void loadExams(); // [1] 加载考试
    void loadUnits(const QString &examCode); // [2] 加载考试单元
    void loadSites(const QString &examCode); // [2] 加载考点
    void loadRooms(const QString &examCode, const QString &siteCode); // [3] 加载考点下的考场

    void loadExamUnitAndSiteAndRoom(const QString &examCode); // @Deprecated: 从服务器加载考试单元、考点、考场

    void loadStudents();   // 加载学生信息
    void loadServerTime(); // 请求服务器的时间
    void initializeSignInStatus(const QList<Student> &students); // 初始化学生的签到信息
    void showPerson(const Person &p);                            // 显示考生的身份证信息

    SignInInfo getSignInInfo(bool validateIdCard = true) const; // 获取签到的学生的信息
    QString getIdCardPicturePath(const SignInInfo &info) const; // 获取签到的学生的身份证照片路径
    QString getFacePicturePath(const SignInInfo &info)   const; // 获取签到的学生的摄像头照片路径
    QString getWritePicturePath(const SignInInfo &info)  const; // 获取签到的学生手写笔记照片路径

    void signIn(const SignInInfo &info) const; // 签到

    Ui::AiSignWidget *ui;
    AiSignWidgetPrivate *d;

    friend struct AiSignWidgetPrivate;
};

#endif // AISIGNWIDGET_H
