#include "ui_CreateUserWidget.h"
#include "CreateUserWidget.h"
#include "bean/User.h"
#include "bean/Permission.h"
#include "dao/UserDao.h"

#include <QDebug>
#include <QMessageBox>
#include <QStandardItem>

AddUserWidget::AddUserWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CreateUserWidget) {
    initialize();
    handleEvents();
}

AddUserWidget::~AddUserWidget() {
    delete ui;
}

// 初始化
void AddUserWidget::initialize() {
    ui->setupUi(this);

    // 存储权限信息的 model
    permissionsModel = new QStandardItemModel(this);
    permissionsModel->setHorizontalHeaderLabels(QStringList() << "权限");

    // model 中添加所有的权限以供创建用户时选择
    foreach (PermissionFlag p, Permission::PERMISSIONS.keys()) {
        QStandardItem *item = new QStandardItem(Permission::PERMISSIONS.value(p));
        item->setData((int) p);
        item->setCheckable(true);
        permissionsModel->appendRow(item);
    }

    ui->tableView->setModel(permissionsModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

// 信号槽事件处理
void AddUserWidget::handleEvents() {
    connect(ui->createButton, &QPushButton::clicked, this, &AddUserWidget::createUser);
}

// 创建用户，成功创建后发射信号 userCreated
void AddUserWidget::createUser() {
    // 1. 校验账号、密码: 账号不能为空，密码不能为空，2 次输入的密码必须一致
    // 2. 获取权限
    // 3. 创建用户
    QString username   = ui->usernameEdit->text().trimmed();
    QString password   = ui->passwordEdit->text();
    QString repassword = ui->repasswordEdit->text();
    QString creator    = ui->creatorComboBox->currentText(); // 操作员的名字

    // [1] 校验账号、密码: 账号不能为空，密码不能为空，2 次输入的密码必须一致
    if (username.isEmpty()) {
        QMessageBox::information(this, "创建用户", "账号不能为空");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::information(this, "创建用户", "密码不能为空");
        return;
    }

    if (password != repassword) {
        QMessageBox::information(this, "创建用户", "2 次输入密码不一致");
        return;
    }

    if (UserDao::isUsernameUsed(username)) {
        QMessageBox::information(this, "创建用户", "账号已经被使用");
        return;
    }

    // [2] 获取权限
    Permission permission;
    for (int row = 0; row < permissionsModel->rowCount(); ++row) {
        QStandardItem *item = permissionsModel->item(row, 0);
        if (item->checkState() == Qt::Checked) {
            permission.addPermission(PermissionFlag(item->data().toInt()));
        }
    }

    if (permission.getPermissionsValue() == 0) {
        QMessageBox::information(this, "创建用户", "没有选中权限");
        return;
    }

    // [3] 创建用户
    User user;
    user.username = username;
    user.password = password;
    user.creator  = creator;
    user.permission = permission;

    UserDao::insertUser(user);
    emit userCreated(username);
}
