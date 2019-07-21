#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QString>
#include <QtSql>

#include "util/Singleton.h"

/**
 * 实现一个简易的数据库连接池，简化数据库连接的获取和释放。连接使用完后不需要手动关闭，数据库连接池会在使用此连接的线程结束后自动关闭连接。
 *
 * 由于安全的原因，大概是 Qt 5.4 以后一个线程创建的连接不允许在其他线程中使用 (早一些的版本可以)。一个线程内的函数执行总是串行的，
 * 绝大多数时候一个线程内使用一个数据库连接就可以了，特殊情况下需要多个连接时传入不同的连接名可以获取到不同的数据库连接。
 *
 * Qt 程序一般不需要去控制连接数，有以下理由:
 * 1. Qt 程序一般都是客户端的桌面程序，同一个程序中不太可能同时创建很多数据库连接，例如 100 个，如果真有，那么就可以考虑下设计是否合理
 * 2. Qt 很少用来开发服务器端程序，访问数据库向前端提供服务
 * 3. Qt 程序可能访问本地的 Sqlite 数据库的情况更多一些，作为客户端时直接访问远程的 MySQL 等安全上时不允许的，数据库一般都不允许外网访问
 * 基于上面的这些原因，一个简单的数据库连接池就能够满足大多数 Qt 程序的需求了。
 *
 * 使用方法:
 * 1. 从数据库连接池里取得连接
 *    QSqlDatabase db = ConnectionPool::instance().openConnection();
 *
 * 2. 使用 db 访问数据库，如
 *    QSqlQuery query(db);
 *
 * 3. 数据库连接使用完后啥都不用管，不需要我们调用 QSqlDatabase::removeDatabase(ConnectionName) 进行关闭
 *
 * 关于 testOnBorrow 的说明:
 *     如果 testOnBorrow 为 true，则连接断开后会自动重新连接 (例如数据库程序崩溃了，网络的原因导致连接断了等)，
 *     但是每次获取连接的时候都会先访问一下数据库测试连接是否有效，如果无效则重新建立连接。testOnBorrow 为 true 时，
 *     需要提供一条 SQL 语句用于测试查询，例如 MySQL 下可以用 SELECT 1 FROM dual，SQLite 可以使用 SELECT 1。
 *
 *     如果 testOnBorrow 为 false，则连接断开后不会自动重新连接，这时获取到的连接调用 QSqlDatabase::isOpen() 返回的值
 *     仍然是 true (因为先前的时候已经建立好了连接，Qt 里没有提供判断底层连接断开的方法或者信号)。
 */

class QSqlDatabase;
class ConnectionPoolPrivate;

class ConnectionPool {
    SINGLETON(ConnectionPool)

public:
    /**
     * @brief 获取数据库连接，连接使用完后不需要手动关闭，数据库连接池会在使用此连接的线程结束后自动关闭连接。
     * 传入的连接名 connectionName 默认为空 (内部会为连接名基于线程的信息创建一个唯一的前缀)，
     * 如果同一个线程需要使用多个不同的数据库连接，可以传入不同的 connectionName
     *
     * @param connectionName 连接的名字
     * @return 返回数据库连接
     */
    QSqlDatabase openConnection(const QString &connectionName = QString());

private:
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接
    ConnectionPoolPrivate *d;
};

#endif // CONNECTIONPOOL_H
