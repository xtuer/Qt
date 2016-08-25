#include "ConfigUtil.h"
#include "JsonReader.h"

#include <QString>
#include <QStringList>

ConfigUtil::ConfigUtil() {
    config = new JsonReader("resources/config.json"); // 配置文件路径
}

ConfigUtil::~ConfigUtil() {

}

void ConfigUtil::release() {
    delete config;
    config = NULL;
}

QString ConfigUtil::getDatabaseType() const {
    return config->getString("database.type");
}

QString ConfigUtil::getDatabaseHost() const {
    return config->getString("database.host");
}

QString ConfigUtil::getDatabaseName() const {
    return config->getString("database.database_name");
}

QString ConfigUtil::getDatabaseUsername() const {
    return config->getString("database.username");
}

QString ConfigUtil::getDatabasePassword() const {
    return config->getString("database.password");
}

bool ConfigUtil::getDatabaseTestOnBorrow() const {
    return config->getBool("database.test_on_borrow", false);
}

QString ConfigUtil::getDatabaseTestOnBorrowSql() const {
    return config->getString("database.test_on_borrow_sql", "SELECT 1");
}

int ConfigUtil::getDatabaseMaxWaitTime() const {
    return config->getInt("database.max_wait_time", 5000);
}

int ConfigUtil::getDatabaseMaxConnectionCount() const {
    return config->getInt("database.max_connection_count", 5);
}

int ConfigUtil::getDatabasePort() const {
    return config->getInt("database.port", 0);
}

bool ConfigUtil::isDatabaseDebug() const {
    return config->getBool("database.debug", false);
}

QStringList ConfigUtil::getDatabaseSqlFiles() const {
    return config->getStringList("database.sql_files");
}

QStringList ConfigUtil::getQssFiles() const {
    return config->getStringList("qss_files");
}
