#include "AddUserWidget.h"
#include "ui_AddUserWidget.h"
#include "Permission.h"

#include <QDebug>
#include <QStandardItem>

AddUserWidget::AddUserWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AddUserWidget) {
    ui->setupUi(this);
    permissionsModel = new QStandardItemModel(this);
    permissionsModel->setHorizontalHeaderLabels(QStringList() << "权限");

    foreach (PermissionFlag p, Permission::PERMISSIONS.keys()) {
        QStandardItem *item = new QStandardItem(Permission::PERMISSIONS.value(p));
        item->setData((int) p);
        item->setCheckable(true);
        permissionsModel->appendRow(item);
    }

    ui->tableView->setModel(permissionsModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();

    connect(ui->createButton, &QPushButton::clicked, this, &AddUserWidget::createUser);
}

AddUserWidget::~AddUserWidget() {
    delete ui;
}

void AddUserWidget::createUser() {
    // 1. 校验账号、密码
    // 2. 获取权限
    // 3. 创建用户
    QString username   = ui->usernameEdit->text().trimmed();
    QString password   = ui->passwordEdit->text();
    QString repassword = ui->repasswordEdit->text();

    // [1] 校验账号、密码
    if (username.isEmpty()) {
        qDebug() << "账号不能为空";
        return;
    }

    if (password.isEmpty()) {
        qDebug() << "密码不能为空";
        return;
    }

    if (password != repassword) {
        qDebug() << "2 次输入密码不一致";
        return;
    }

    // [2] 获取权限
    Permission p;
    for (int row = 0; row < permissionsModel->rowCount(); ++row) {
        QStandardItem *item = permissionsModel->item(row, 0);
        if (item->checkState() == Qt::Checked) {
            p.addPermission(PermissionFlag(item->data().toInt()));
        }
    }

    if (p.getPermissionsValue() == 0) {
        qDebug() << "没有选中权限";
        return;
    }

    qDebug() << p.getPermissions() << p.getPermissionsValue();

    // [3] 创建用户
}
