#include "ConnectionPool.h"
#include "util/Config.h"

#include <QDebug>
#include <QString>
#include <QThread>
#include <QCoreApplication>

/*-----------------------------------------------------------------------------|
 |                          ConnectionPoolPrivate 的定义                        |
 |----------------------------------------------------------------------------*/
class ConnectionPoolPrivate {
public:
    ConnectionPoolPrivate();

    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    int     port;

    bool    testOnBorrow;    // 取得连接的时候验证连接有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL
};

ConnectionPoolPrivate::ConnectionPoolPrivate() {
    Config &config = Singleton<Config>::getInstance();

    // 从配置文件里读取
    hostName        = config.getDatabaseHost();
    databaseName    = config.getDatabaseName();
    username        = config.getDatabaseUsername();
    password        = config.getDatabasePassword();
    databaseType    = config.getDatabaseType();
    port            = config.getDatabasePort();
    testOnBorrow    = config.getDatabaseTestOnBorrow();
    testOnBorrowSql = config.getDatabaseTestOnBorrowSql();
}

/*-----------------------------------------------------------------------------|
 |                             ConnectionPool 的定义                            |
 |----------------------------------------------------------------------------*/
ConnectionPool::ConnectionPool() : d(new ConnectionPoolPrivate) {
}

ConnectionPool::~ConnectionPool() {
    delete d;
}

// 获取数据库连接
QSqlDatabase ConnectionPool::openConnection(const QString &connectionName) {
    // 1. 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    // 2. 如果连接已经存在，复用它，而不是重新创建
    //    2.1 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
    // 3. 如果连接不存在，则创建连接
    // 4. 线程结束时，释放在此线程中创建的数据库连接

    // [1] 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    QString baseConnectionName = "conn_" + QString::number(quint64(QThread::currentThread()), 16);
    QString fullConnectionName = baseConnectionName + connectionName;

    if (QSqlDatabase::contains(fullConnectionName)) {
        // [2] 如果连接已经存在，复用它，而不是重新创建
        QSqlDatabase existingDb = QSqlDatabase::database(fullConnectionName);

        if (d->testOnBorrow) {
            // [2.1] 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
            qDebug().noquote() << QString("Test connection on borrow, execute: %1, for connection %2")
                                  .arg(d->testOnBorrowSql).arg(fullConnectionName);
            QSqlQuery query(d->testOnBorrowSql, existingDb);

            if (query.lastError().type() != QSqlError::NoError && !existingDb.open()) {
                qDebug().noquote() << "Open datatabase error:" << existingDb.lastError().text();
                return QSqlDatabase();
            }
        }

        return existingDb;
    } else {
        // [3] 如果连接不存在，则创建连接
        if (qApp != nullptr) {
            // [4] 线程结束时，释放在此线程中创建的数据库连接
            QObject::connect(QThread::currentThread(), &QThread::finished, qApp, [fullConnectionName] {
                if (QSqlDatabase::contains(fullConnectionName)) {
                    QSqlDatabase::removeDatabase(fullConnectionName);
                    qDebug().noquote() << QString("Connection deleted: %1").arg(fullConnectionName);
                }
            });
        }

        return createConnection(fullConnectionName);
    }
}

// 创建数据库连接
QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    static int sn = 0;

    // 创建一个新的数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase(d->databaseType, connectionName);
    db.setHostName(d->hostName);
    db.setDatabaseName(d->databaseName);
    db.setUserName(d->username);
    db.setPassword(d->password);

    if (d->port != 0) {
        db.setPort(d->port);
    }

    if (db.open()) {
        qDebug().noquote() << QString("Connection created: %1, sn: %2").arg(connectionName).arg(++sn);
        return db;
    } else {
        qDebug().noquote() << "Create connection error:" << db.lastError().text();
        return QSqlDatabase();
    }
}

