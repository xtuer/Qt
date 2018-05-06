#include "ui_UsersWidget.h"
#include "UsersWidget.h"
#include "CreateUserWidget.h"
#include "bean/User.h"
#include "bean/Permission.h"
#include "dao/UserDao.h"

#include <QList>
#include <QMessageBox>

static const int ROLE_ID = Qt::UserRole + 2;
static const int ROLE_PERMISSION = Qt::UserRole + 3;

UsersWidget::UsersWidget(QWidget *parent) : QWidget(parent), ui(new Ui::UsersWidget) {
    initialize();
    handleEvents();
    showUsers();
}

UsersWidget::~UsersWidget() {
    delete ui;
}

// 初始化
void UsersWidget::initialize() {
    ui->setupUi(this);

    usersModel = new QStandardItemModel(this);
    permissionsModel = new QStandardItemModel(this);
    usersModel->setHorizontalHeaderLabels(QStringList() << "账号");
    permissionsModel->setHorizontalHeaderLabels(QStringList() << "权限");

    ui->usersTableView->setModel(usersModel);
    ui->usersTableView->horizontalHeader()->setStretchLastSection(true);
    ui->usersTableView->verticalHeader()->hide();
    ui->usersTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->permissionsTableView->setModel(permissionsModel);
    ui->permissionsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->permissionsTableView->verticalHeader()->hide();
    ui->permissionsTableView->setSelectionMode(QAbstractItemView::NoSelection);
}

// 信号槽事件处理
void UsersWidget::handleEvents() {
    // 创建用户
    connect(ui->createButton, &QPushButton::clicked, [this] {
        AddUserWidget *w = new AddUserWidget;
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

        // 创建用户后显示到用户列表里
        connect(w, &AddUserWidget::userCreated, [this] (const QString &username) {
            User user = UserDao::findUserByUsername(username);

            if (0 != user.id) {
                usersModel->appendRow(createUserItem(user));
            }
        });
    });

    // 删除用户
    connect(ui->deleteButton, &QPushButton::clicked, [this] {
        // 获取当前选中的 index
        QItemSelectionModel *sm = ui->usersTableView->selectionModel();
        QModelIndexList indexes = sm->selectedIndexes();
        QModelIndex index = indexes.count() > 0 ? indexes.at(0) : QModelIndex();

        if (!index.isValid()) { return; }

        int userId = index.data(ROLE_ID).toInt();
        QString username = index.data().toString().trimmed();

        if ("admin" == username) {
            QMessageBox::information(this, "删除用户", "不能删除管理员");
            return;
        }

        if (QMessageBox::Yes != QMessageBox::question(this, "删除用户", QString("确定删除 %1 吗?").arg(username))) {
            return;
        }

        // 成功删除用户后从用户列表里删除，并清除右边的用户权限
        if (UserDao::deleteUser(userId) > 0) {
            usersModel->removeRow(index.row());
            permissionsModel->removeRows(0, permissionsModel->rowCount());
        }
    });

    // 点击用户，右边显示用户的权限
    connect(ui->usersTableView, &QTableView::clicked, [this] (const QModelIndex &index) {
        int permissionValue = index.data(ROLE_PERMISSION).toInt();
        Permission permission = Permission(PermissionFlags(permissionValue));
        showPermissions(permission);
    });
}

// 显示用户
void UsersWidget::showUsers() {
    QList<User> users = UserDao::findAllUsers();;
    // users << User(1, "Admin", "123456", Permission(PermissionFlag::ADMIN))
    //       << User(2, "Bob",   "123456", Permission(PermissionFlag::CHECK | PermissionFlag::MODIFY_SECURITY))
    //       << User(3, "John",  "123456", Permission(PermissionFlag::AUDIT_TRAIL))
    //       << User(4, "Jack",  "123456", Permission(PermissionFlag::QUIT | PermissionFlag::SIGNATURE));

    // 显示用户
    for (const User &user : users) {
        usersModel->appendRow(createUserItem(user));
    }
}

// 显示权限
void UsersWidget::showPermissions(Permission permission) {
    permissionsModel->removeRows(0, permissionsModel->rowCount());

    foreach (PermissionFlag p, Permission::PERMISSIONS.keys()) {
        QStandardItem *item = new QStandardItem(Permission::PERMISSIONS.value(p));
        item->setData((int) p);
        item->setEditable(false);
        item->setCheckState(permission.hasPermission(p) ? Qt::Checked : Qt::Unchecked);
        permissionsModel->appendRow(item);
    }
}

// 创建用户的 item
QStandardItem *UsersWidget::createUserItem(const User &user) const {
    QStandardItem *item = new QStandardItem(user.username);
    item->setData(user.id, ROLE_ID);
    item->setData(user.permission.getPermissionsValue(), ROLE_PERMISSION);
    item->setEditable(false);

    return item;
}
