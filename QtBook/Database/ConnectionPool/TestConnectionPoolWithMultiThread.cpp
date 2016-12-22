#include "TestConnectionPoolWithMultiThread.h"
#include "ConnectionPool.h"
#include <QtSql>

TestConnectionPoolWithMultiThread::TestConnectionPoolWithMultiThread(int sn)  : sn(sn) { }

void TestConnectionPoolWithMultiThread::run() {
    // 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    qDebug() << QString("In thread %1 run() with connection: %2").arg(sn).arg(db.connectionName());

    QSqlQuery query(db);
    query.exec("SELECT * FROM user where id=1");

    while (query.next()) {
        qDebug() << query.value("username").toString();
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

