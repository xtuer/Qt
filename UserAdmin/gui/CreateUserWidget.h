#ifndef CREATEUSERWIDGET_H
#define CREATEUSERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class CreateUserWidget;
}

class AddUserWidget : public QWidget {
    Q_OBJECT

public:
    explicit AddUserWidget(QWidget *parent = 0);
    ~AddUserWidget();

signals:
    void userCreated(const QString &username); // 创建用户后发射的信号

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理
    void createUser();   // 创建用户，成功创建后发射信号 userCreated

    Ui::CreateUserWidget *ui;
    QStandardItemModel *permissionsModel; // 权限的 model
};

#endif // CREATEUSERWIDGET_H
