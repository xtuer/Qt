#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

class User;
class Permission;

namespace Ui {
class UsersWidget;
}

/**
 * 用户列表 widget
 */
class UsersWidget : public QWidget {
    Q_OBJECT

public:
    explicit UsersWidget(QWidget *parent = 0);
    ~UsersWidget();

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理
    void showUsers();    // 显示用户
    void showPermissions(Permission permission); // 显示权限
    QStandardItem *createUserItem(const User &user) const; // 创建用户的 item

    Ui::UsersWidget *ui;
    QStandardItemModel *usersModel;
    QStandardItemModel *permissionsModel;
};

#endif // USERSWIDGET_H
