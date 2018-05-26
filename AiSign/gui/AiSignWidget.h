#ifndef AISIGNWIDGET_H
#define AISIGNWIDGET_H

#include <QWidget>

struct AiSignWidgetPrivate;
class Student;

namespace Ui {
class AiSignWidget;
}

class AiSignWidget : public QWidget {
    Q_OBJECT

public:
    explicit AiSignWidget(QWidget *parent = 0);
    ~AiSignWidget();

private slots:
    void showInfo(const QString &info, bool error = false); // 显示信息, error 为 true 时以红色显示

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理

    void loadPeriodUnitAndSiteAndRoom();    // 从服务器加载考期、考点、考场
    void loadRoom(const QString &siteCode); // 加载考点下的考场
    void loadStudents();   // 加载学生信息
    void loadServerTime(); // 请求服务器的时间
    void updateSignInStatus(const QList<Student> &students); // 更新学生的签到信息

    void signInManually(); // 人工签到

    Ui::AiSignWidget *ui;
    AiSignWidgetPrivate *d;

    friend struct AiSignWidgetPrivate;
};

#endif // AISIGNWIDGET_H
