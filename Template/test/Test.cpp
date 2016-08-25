#include "Test.h"
#include "util/SqlUtil.h"
#include "util/UiUtil.h"

#include "bean/User.h"
#include "dao/UserDao.h"
#include "dao/DaoTemplate.h"

#include <QList>
#include <QDebug>

#define START_TEST(methodName) qDebug() << QString("----------Start Test: %1()----------").arg(#methodName);
#define END_TEST(methodName)   qDebug() << QString("----------End   Test: %1()----------").arg(#methodName) << "\n\n";

void Test::test() {
//    testReadQss();
//    testReadSql();
    testUserDao();
}

void Test::testReadQss() {
    START_TEST(testReadQss);
    UiUtil::loadQss();
    END_TEST(testReadQss);
}

void Test::testReadSql() {
    START_TEST(testReadSql);

    qDebug() << SqlUtil::getInstance().getSql("User", "selectById");
    qDebug() << SqlUtil::getInstance().getSql("User", "selectAll");
    qDebug() << SqlUtil::getInstance().getSql("Product", "selectById");
    qDebug() << SqlUtil::getInstance().getSql("Product", "selectAll");
    qDebug() << SqlUtil::getInstance().getSql("Product", "selectByCompanyName");

    END_TEST(testReadSql);
}

void Test::testUserDao() {
    START_TEST(testUserDao);

    User user = UserDao::selectById(1);
    qDebug() << QString("Username: %1, Password: %2").arg(user.username).arg(user.password);

    qDebug() << "Select all users.";
    QList<User> users = UserDao::selectAll();
    foreach (User u, users) {
        qDebug() << QString("Username: %1, Password: %2").arg(u.username).arg(u.password);
    }

//    qDebug() << "Inser new user";
//    User newUser;
//    newUser.username = "Avatar";
//    newUser.password = "Passw0rd";
//    UserDao::insert(newUser);

    qDebug() << "Update user";
    User newUser;
    newUser.id = 2;
    newUser.username = "Avatar";
    newUser.password = "Secret";
    UserDao::update(newUser);

    QString sql("SELECT id, username, password FROM user");
    qDebug() << DaoTemplate::selectMaps(sql);

    END_TEST(testUserDao);
}
