#ifndef DBUTIL_H
#define DBUTIL_H

#include <QtSql>
#include <QString>

class DBUtil {
public:
    static QSqlDatabase getDatabase(const QString &connectionName = "DefaultConnection"); // 创建数据库连接
    static void removeDatabase(const QString &connectionName = "DefaultConnection");      // 删除数据库连接
};

#endif // DBUTIL_H
 
