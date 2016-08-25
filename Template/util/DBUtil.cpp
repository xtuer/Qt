#include "DBUtil.h"
#include "util/SettingUtil.h"
#include "info/Error.h"
#include <QDebug>

QSqlDatabase DBUtil::getDatabase(const QString& connectionName) {
    if (QSqlDatabase::contains(connectionName)) {
        return QSqlDatabase::database(connectionName);
    }

    QString databaseType = SettingUtil::getInstance().getDatabaseType().toUpper();
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);

    if ("QSQLITE" == databaseType) {
        db.setDatabaseName(SettingUtil::getInstance().getDatabaseName());
    } else if ("QMYSQL" == databaseType) {
        db.setHostName(SettingUtil::getInstance().getDatabaseHost());     // "127.0.0.1"
        db.setDatabaseName(SettingUtil::getInstance().getDatabaseName()); // test
        db.setUserName(SettingUtil::getInstance().getDatabaseUsername());
        db.setPassword(SettingUtil::getInstance().getDatabasePassword());
    }

    if (!db.open()) {
        qDebug() << db.lastError().text();
        exit(Error::DB_CANNOT_OPEN); // 最好时错误代码与对应的文字信息关联起来，这样才能找到错误的原因.
    }

    return db;
}

void DBUtil::removeDatabase(const QString &connectionName) {
    QSqlDatabase::removeDatabase(connectionName);
}


