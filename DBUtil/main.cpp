#include "bean/User.h"
#include "dao/UserDao.h"

#include "db/Sqls.h"
#include "db/DBUtil.h"
#include "db/ConnectionPool.h"
#include "util/Config.h"
#include "Thread.h"

#include <QDebug>
#include <QPluginLoader>
#include <QApplication>
#include <QPushButton>

void useDBUtil();
void useSqlFromFile();
void useDao();
void useThreads();
void testOnBorrow();
void loadMySqlDriver();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //    loadMySqlDriver();
    //    useDBUtil();
    //    useSqlFromFile();
    useDao();
    //    useThreads();
    //    testOnBorrow();

    return app.exec();
}

void useDBUtil() {
    // 1. 查找 Alice 的 ID
    qDebug() << "\n1. 查找 Alice 的 ID";
    qDebug() << DBUtil::selectInt("select id from user where username='Alice'");
    qDebug() << DBUtil::selectVariant("select id from user where username='Alice'").toInt();

    // 2. 查找 Alice 的密码
    qDebug() << "\n2. 查找 Alice 的密码";
    qDebug() << DBUtil::selectString("select password from user where username='Alice'");
    qDebug() << DBUtil::selectMap("select password from user where username='Alice'")["password"].toString();

    // 3. 查找 Alice 的所有信息，如名字，密码，邮件等
    qDebug() << "\n3. 查找 Alice 的所有信息，如名字，密码，邮件等";
    qDebug() << DBUtil::selectMap("select * from user where username='Alice'");

    // 4. 查找 Alice 和 Bob 的所有信息，如名字，密码，邮件等
    qDebug() << "\n4. 查找 Alice 和 Bob 的所有信息，如名字，密码，邮件等";
    qDebug() << DBUtil::selectMaps("select * from user where username='Alice' or username='Bob'");

    // 5. 查找 Alice 和 Bob 的密码
    qDebug() << "\n5. 查找 Alice 和 Bob 的密码";
    qDebug() << DBUtil::selectStrings("select password from user where username='Alice' or username='Bob'");

    // 6. 查询时使用命名参数
    qDebug() << "\n6. 查询时使用命名参数";
    QMap<QString, QVariant> params;
    params["id"] = 1;

    qDebug() << DBUtil::selectMap("select * from user where id=:id", params);
    qDebug() << DBUtil::selectString("select username from user where id=:id", params);
}

void useSqlFromFile() {
    // 读取 namespace 为 User 下，id 为 findByUserId 的 SQL 语句
    qDebug() << Sqls::instance().getSql("User", "findByUserId");
    qDebug() << Sqls::instance().getSql("User", "findByUserId");
    qDebug() << Sqls::instance().getSql("User", "findByUserId-1"); // 找不到这条 SQL 语句会有提示
    qDebug() << DBUtil::selectMap(Sqls::instance().getSql("User", "findByUserId").arg(2));
}

void useDao() {
    // 使用基于 DBUtil 封装好的 DAO 查询数据库
    User user = UserDao::findByUserId(1);
    qDebug() << user.username;
    qDebug() << user.toString();

    // 更新数据库
    user.email = "bob@gmail.com";
    qDebug() << "Update: " << UserDao::update(user);

    QList<User> users = UserDao::findtAll();
    foreach (const User &u, users) {
        qDebug() << u.toString();
    }
}

// 测试多线程使用数据库连接池
void useThreads() {
    for (int i = 0; i < 100; ++i) {
        Thread *thread = new Thread();
        thread->start();

        // 如果瞬间启动多个线程建立 MySQL 数据库连接，很可能会报异常 unable to allocate a MYSQL object
        // 导致部分连接建立失败，于是等待 100 毫秒才启动下一个线程
        QThread::msleep(100);
    }
}

// 测试自动重连数据库，可解决数据库崩溃，MySQL 6 个小时自动断开不活跃连接的问题
void testOnBorrow() {
    // 测试步骤:
    // 1. 点击按钮访问数据库: 输出 2
    // 2. 关闭数据库
    // 3. 点击按钮访问数据库: 输出 database not open
    // 4. 启动数据库
    // 5. 点击按钮访问数据库: 输出 2
    QPushButton *button = new QPushButton("On Borrow Test");
    button->show();

    QObject::connect(button, &QPushButton::clicked, [] {
        qDebug() << DBUtil::selectInt("select id from user where username='Bob'");
    });
}

void loadMySqlDriver() {
    QPluginLoader loader;
    // MySQL 驱动插件的路径
    loader.setFileName("/Users/Biao/Qt5.12.4/5.12.4/clang_64/plugins/sqldrivers/libqsqlmysql.dylib");
    qDebug() << loader.load();
    qDebug() << loader.errorString();
}
