#include "DBUtil.h"
#include "SettingsUtil.h"
#include <QDebug>

SINGLETON_INITIALATE(DBUtil)
SINGLETON_DESTRUCTOR(DBUtil)

DBUtil::DBUtil() : connectionName("MyConnection") {
    QString databaseType = SettingsUtil::getDatabaseType().toUpper();
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);

    if ("QSQLITE" == databaseType) {
        db.setDatabaseName(SettingsUtil::getDatabaseName());
    } else if ("QMYSQL" == databaseType) {
        db.setHostName(SettingsUtil::getDatabaseHost());     // "127.0.0.1"
        db.setDatabaseName(SettingsUtil::getDatabaseName()); // test
        db.setUserName(SettingsUtil::getDatabaseUsername());
        db.setPassword(SettingsUtil::getDatabasePassword());
    }

    if (!db.open()) {
        qDebug() << db.lastError().text();
        exit(2); // 最好时错误代码与对应的文字信息关联起来，这样才能找到错误的原因.
    }
}

QSqlDatabase DBUtil::getDatabase() {
    return QSqlDatabase::database(getInstance().connectionName, true);
}

void DBUtil::removeDatabase() {
    QSqlDatabase::removeDatabase(getInstance().connectionName);
}


