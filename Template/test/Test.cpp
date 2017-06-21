#include "Test.h"
#include "util/UiUtil.h"

#include "bean/User.h"
#include "dao/UserDao.h"
#include "db/Sqls.h"
#include "util/Config.h"

#include <QList>
#include <QDebug>

#define START_TEST(methodName) qDebug() << QString("----------Start Test: %1()----------").arg(#methodName);
#define END_TEST(methodName)   qDebug() << QString("----------End   Test: %1()----------").arg(#methodName) << "\n\n";

void Test::test() {
//    testReadQss();
    testReadSql();
//    testUserDao();
    testReadConfig();
}

void Test::testReadQss() {
    START_TEST(testReadQss);
    UiUtil::loadQss();
    END_TEST(testReadQss);
}

void Test::testReadSql() {
    START_TEST(testReadSql);

    qDebug() << Singleton<Sqls>::getInstance().getSql("User", "insert");
    qDebug() << Singleton<Sqls>::getInstance().getSql("User", "findByUserId");

    END_TEST(testReadSql);
}

void Test::testUserDao() {
    START_TEST(testUserDao);

    END_TEST(testUserDao);
}

void Test::testReadConfig() {
    START_TEST(testReadConfig);

    qDebug() << Singleton<Config>::getInstance().getDatabaseName();

    END_TEST(testReadConfig);
}
