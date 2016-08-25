#ifndef DBUTIL_H
#define DBUTIL_H

#include "Singleton.h"
#include <QtSql>

class DBUtil {
    SINGLETON(DBUtil)
public:
    static QSqlDatabase getDatabase();
    static void removeDatabase();

private:
    QString connectionName;
};

#endif // DBUTIL_H
 
