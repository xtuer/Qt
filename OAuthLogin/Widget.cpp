#include "Widget.h"
#include "ui_Widget.h"

#include "HttpClient.h"
#include "Json.h"
#include "Config.h"

#include <QDateTime>
#include <QMap>
#include <QCryptographicHash>
#include <QDebug>

/**
 * 生成签名，登录使用
 */
static QString sign(const QString appId, const QString appKey, const QString username, const QString password, qint64 timestamp) {
    /*
     签名计算逻辑:
     1. 把参数按照参数名字母序排列 (使用 TreeMap)
     2. 参数和参数值使用等号 = 连接，参数对之间使用 & 连接
     3. 对参数拼接得到的字符串求 MD5 即签名
     */
    QString params = QString("appId=%1&appKey=%2&password=%3&timestamp=%4&username=%5")
            .arg(appId)
            .arg(appKey)
            .arg(password)
            .arg(timestamp)
            .arg(username);

    QString md5 = QCryptographicHash::hash(params.toUtf8(), QCryptographicHash::Md5).toHex();
    return md5;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    // 登录
    connect(ui->loginButton, &QPushButton::clicked, [this]() {
        QString username = ui->usernameLineEdit->text().trimmed();
        QString password = ui->passwordLineEdit->text();
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        QString loginSign = sign(Config::appId, Config::appKey, username, password, timestamp);

        // 修改 LoginWidget::handleEvents 的 ui->loginButton 点击事件，在事件中调用下面的代码进行登录
        HttpClient(Config::URL_LOGIN).debug(true)
                .header("X-Requested-With", "XMLHttpRequest")
                .param("appId", Config::appId)
                .param("username", username)
                .param("password", password)
                .param("timestamp", timestamp)
                .param("sign", loginSign)
                .success([this](const QString &response) {
                    Json json(response);
                    if (json.getBool("success")) {
                        // 创建登录用户
                        loginUser.id = json.getString("data.userId");
                        loginUser.username = json.getString("data.username");
                        loginUser.orgId = json.getString("data.orgId");
                        loginUser.permission = Permission(PermissionFlags(json.getInt("data.permission")));
                        loginUser.deviceEnabled = json.getBool("data.deviceEnabled");
                        loginUser.devices = QStringList();

                        // 获取用户设备
                        for (QString device : json.getString("data.devices").split(',', Qt::SkipEmptyParts)) {
                            loginUser.devices << device.trimmed();
                        }

                        // 保存登录成功的 token，后面访问服务器都需要这个 token，不再使用用户名密码
                        loginUser.authToken = json.getString("data.authToken");

                        qDebug() << loginUser;

                        // 登录成功后调用 UiUtil::topLevelWidget(this)->close(); 关闭登录窗口
                    } else {
                        qDebug() << "登录失败";
                    }
                })
                .fail([this](const QString &response, int code) {
                    // 提示: 连接不上服务器回调此函数
                    qDebug() << "连接服务器失败";
                }).post();
    });

    // 修改权限: 登录成功后才能修改权限
    connect(ui->changePermissionButton, &QPushButton::clicked, [this]() {
        int permission = ui->permissionLineEdit->text().trimmed().toInt();

        // userId 为要修改权限的用户 ID
        QString url = QString(Config::URL_PERMISSION_CHANGE).replace("{userId}", loginUser.id);

        HttpClient(url).debug(true)
                .header("X-Requested-With", "XMLHttpRequest")
                .header("auth-token", loginUser.authToken) // 需要登录 token 才能访问
                .param("permission", permission)
                .param("_method", "PATCH")
                .success([=, this](const QString &response) {
                    Json json(response);
                    if (json.getBool("success")) {
                        qDebug() << "修改权限成功";
                        loginUser.permission = Permission(PermissionFlags(permission));
                    } else {
                        qDebug() << "修改权限失败";
                    }
                }).post();
    });

    // 修改密码: 登录成功后才能修改密码
    connect(ui->changePasswordButton, &QPushButton::clicked, [this]() {
        QString newPassword = ui->newPasswordLineEdit->text();

        // userId 为要修改密码的用户的 ID，可以是当前用户的 ID，也可以是其他用户的 ID
        QString url = QString(Config::URL_PASSWORD_CHANGE).replace("{userId}", loginUser.id);

        HttpClient(url).debug(true)
                .header("X-Requested-With", "XMLHttpRequest")
                .header("auth-token", loginUser.authToken) // 需要登录 token 才能访问
                .param("value", newPassword)
                .success([=, this](const QString &response) {
                    Json json(response);
                    if (json.getBool("success")) {
                        qDebug() << "密码修改权限成功";
                    } else {
                        qDebug() << "密码修改权限失败";
                    }
                }).put();
    });

    // 获取机构用户: 获取当前登录用户所属机构的所有用户
    connect(ui->findOrgUsersButton, &QPushButton::clicked, [this]() {
        QString url = QString(Config::URL_USERS_OF_ORG).replace("{orgId}", loginUser.orgId);

        HttpClient(url).debug(true)
                .header("X-Requested-With", "XMLHttpRequest")
                .header("auth-token", loginUser.authToken) // 需要登录 token 才能访问
                .success([=, this](const QString &response) {
                    Json json(response);
                    if (json.getBool("success")) {
                        qDebug() << "获取机构用户成功";

                        QJsonArray array = json.getJsonArray("data");
                        for (QJsonArray::const_iterator iter = array.constBegin(); iter != array.constEnd(); ++iter) {
                            QJsonValue value = *iter;
                            QJsonObject userObject = value.toObject();

                            // 从响应中获取用户
                            User user;
                            user.id = json.getString("userId", "", userObject);
                            user.username = json.getString("username", "", userObject);
                            user.permission = Permission(PermissionFlags(json.getInt("permission", 0, userObject)));
                            qDebug() << user;
                        }
                    } else {
                        qDebug() << "获取机构用户失败";
                    }
                }).get();
    });

    // 创建机构用户
    connect(ui->createUserButton, &QPushButton::clicked, [this]() {
        QString url = QString(Config::URL_USERS_OF_ORG).replace("{orgId}", loginUser.orgId);
        Json json("{}");
        json.set("username", "Hello1"); // 同一个机构下 username 不能重复
        json.set("nickname", "名字随意");
        json.set("password", "123456");
        json.set("roles", QStringList() << "ROLE_USER"); // 不变

        HttpClient(url).debug(true)
                .header("X-Requested-With", "XMLHttpRequest")
                .header("auth-token", loginUser.authToken) // 需要登录 token 才能访问
                .json(json.toString())
                .success([=, this](const QString &response) {
                    Json json(response);

                    if (json.getBool("success")) {
                        User user;
                        user.id = json.getString("data.userId"); // data 为服务器返回的新创建的用户信息，例如 userId 为服务器生成的
                        user.username = json.getString("data.username");
                        user.orgId = json.getString("data.orgId");
                        user.permission = Permission(PermissionFlags(json.getInt("data.permission")));

                        // 保存登录成功的 token，后面访问服务器都需要这个 token，不再使用用户名密码
                        user.authToken = json.getString("data.authToken");

                        qDebug() << user;
                    } else {
                        qDebug() << "创建机构用户失败";
                    }
                }).post();
    });
}

Widget::~Widget() {
    delete ui;
}

