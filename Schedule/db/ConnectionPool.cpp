#include "ConnectionPool.h"
#include "util/ConfigUtil.h"
#include <QDebug>
#include <QtSql>
#include <QStack>
#include <QString>
#include <QMutex>
#include <QSemaphore>
#include <QApplication>

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                         ConnectionPoolPrivate 的定义                      ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////
class ConnectionPoolPrivate {
public:
    ConnectionPoolPrivate();
    ~ConnectionPoolPrivate();

    QStack<QString> usedConnectionNames;   // 已使用的数据库连接名
    QStack<QString> unusedConnectionNames; // 未使用的数据库连接名

    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    int port;

    bool    testOnBorrow;    // 取得连接的时候验证连接有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL
    int maxWaitTime;         // 获取连接最大等待时间
    int maxConnectionCount;  // 最大连接数

    QSemaphore *semaphore;

    static QMutex mutex;
    static int lastKey; // 用来创建连接的名字，保证连接名字不会重复
};

QMutex ConnectionPoolPrivate::mutex;
int ConnectionPoolPrivate::lastKey = 0;

ConnectionPoolPrivate::ConnectionPoolPrivate() {
    ConfigUtil &config = Singleton<ConfigUtil>::getInstance();

    // 从配置文件里读取
    hostName           = config.getDatabaseHost();
    databaseName       = config.getDatabaseName();
    username           = config.getDatabaseUsername();
    password           = config.getDatabasePassword();
    databaseType       = config.getDatabaseType();
    port               = config.getDatabasePort();
    testOnBorrow       = config.getDatabaseTestOnBorrow();
    testOnBorrowSql    = config.getDatabaseTestOnBorrowSql();
    maxWaitTime        = config.getDatabaseMaxWaitTime();
    maxConnectionCount = config.getDatabaseMaxConnectionCount();

    semaphore = new QSemaphore(maxConnectionCount);
}

ConnectionPoolPrivate::~ConnectionPoolPrivate() {
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    delete semaphore;
}

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                           ConnectionPool 的定义                           ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////
ConnectionPool::ConnectionPool() : data(new ConnectionPoolPrivate) {
    // 程序结束前释放链接
    QObject::connect(qApp, &QApplication::aboutToQuit, [this] {
        release();
    });
}

ConnectionPool::~ConnectionPool() {
    delete data;
}

QSqlDatabase ConnectionPool::openConnection() {
    Q_ASSERT(NULL != data);

    if (data->semaphore->tryAcquire(1, data->maxWaitTime)) {
        // 有已经回收的连接，复用它们
        // 没有已经回收的连接，则创建新的连接
        ConnectionPoolPrivate::mutex.lock();
        QString connectionName = data->unusedConnectionNames.size() > 0 ?
                    data->unusedConnectionNames.pop() :
                    QString("C%1").arg(++ConnectionPoolPrivate::lastKey);
        data->usedConnectionNames.push(connectionName);
        ConnectionPoolPrivate::mutex.unlock();

        // 创建连接，因为创建连接很耗时，所以不放在 lock 的范围内，提高并发效率
        QSqlDatabase db = createConnection(connectionName);

        if (!db.isOpen()) {
            ConnectionPoolPrivate::mutex.lock();
            data->usedConnectionNames.removeOne(connectionName); // 无效连接删除
            ConnectionPoolPrivate::mutex.unlock();

            data->semaphore->release(); // 没有消耗连接
        }

        return db;
    } else {
        // 创建连接超时，返回一个无效连接
        qDebug() << "Time out to create connection.";
        return QSqlDatabase();
    }
}

void ConnectionPool::closeConnection(const QSqlDatabase &connection) {
    Q_ASSERT(NULL != data);
    QString connectionName = connection.connectionName();

    // 如果是我们创建的连接，并且已经被使用，则从 used 里删除，放入 unused 里
    if (data->usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&ConnectionPoolPrivate::mutex);
        data->usedConnectionNames.removeOne(connectionName);
        data->unusedConnectionNames.push(connectionName);
        data->semaphore->release();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    Q_ASSERT(NULL != data);

    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (data->testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << QString("Test connection on borrow, execute: %1, for connection %2")
                        .arg(data->testOnBorrowSql).arg(connectionName);
            QSqlQuery query(data->testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(data->databaseType, connectionName);
    db.setHostName(data->hostName);
    db.setDatabaseName(data->databaseName);
    db.setUserName(data->username);
    db.setPassword(data->password);

    if (data->port != 0) {
        db.setPort(data->port);
    }

    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}

void ConnectionPool::release() {
    Q_ASSERT(NULL != data);

    if (NULL != data) {
        ConnectionPoolPrivate::mutex.lock();
        delete data;
        data = NULL;
        ConnectionPoolPrivate::mutex.unlock();

        qDebug() << "Release connection pool";
    }
}
