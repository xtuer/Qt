#include "bean/User.h"
#include "dao/UserDao.h"

#include "db/Sqls.h"
#include "db/DBUtil.h"
#include "db/ConnectionPool.h"
#include "util/Config.h"

#include <QDebug>

void useDBUtil();
void useSqlFromFile();
void useDao();

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    useDBUtil();
//    useSqlFromFile();
//    useDao();

    Singleton<ConnectionPool>::getInstance().destroy(); // 销毁连接池，释放数据库连接

    return 0;
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
    qDebug() << Singleton<Sqls>::getInstance().getSql("User", "findByUserId");
    qDebug() << Singleton<Sqls>::getInstance().getSql("User", "findByUserId-1"); // 找不到这条 SQL 语句会有提示
    qDebug() << DBUtil::selectMap(Singleton<Sqls>::getInstance().getSql("User", "findByUserId").arg(2));
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
