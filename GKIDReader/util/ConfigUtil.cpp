#include "ConfigUtil.h"
#include "JsonReader.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QWidget>

ConfigUtil::ConfigUtil() {
    config = new JsonReader(QString("data/config.json")); // 配置文件路径
    guiSettings = new QSettings("data/gui.ini", QSettings::IniFormat);
    guiSettings->setIniCodec("UTF-8");
}

ConfigUtil::~ConfigUtil() {

}

void ConfigUtil::release() {
    delete config;
    config = NULL;

    if (NULL != guiSettings) {
        guiSettings->sync(); // 保存配置
    }

    delete guiSettings;
    guiSettings = NULL;
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

int ConfigUtil::getLayoutPadding() const {
    return config->getInt("ui.padding", 10);
}

int ConfigUtil::getLayoutSpacing() const {
    return config->getInt("ui.spacing", 5);
}

void ConfigUtil::saveWindowGeometry(const QString &groupName, QWidget *window) {
    setGuiValue(groupName, "pos", window->pos());
    setGuiValue(groupName, "size", window->rect().size());
}

void ConfigUtil::restoreWindowGeometry(const QString &groupName, QWidget *window) {
    window->move(getGuiValue(groupName, "pos", QPoint(0, 0)).toPoint());
    window->resize(getGuiValue(groupName, "size", QSize(600, 400)).toSize());
}

QString ConfigUtil::getLoginUrl() const {
    return config->getString("loginUrl");
}

QString ConfigUtil::getTimeServiceUrl() const {
    return config->getString("timeServiceUrl");
}

bool ConfigUtil::isDebug() const {
    return config->getBool("debug");
}

QVariant ConfigUtil::getGuiValue(const QString& groupName, const QString& name, const QVariant& def) {
    return getValue(guiSettings, groupName, name, def);
}

void ConfigUtil::setGuiValue(const QString& groupName, const QString& name, const QVariant& value) {
    setValue(guiSettings, groupName, name, value);
}

QVariant ConfigUtil::getValue(QSettings* settings, const QString& groupName, const QString& settingName, const QVariant &def) {
    QVariant var;

    settings->beginGroup(groupName);
    var = settings->value(settingName, def);
    settings->endGroup();

    return var;
}

void ConfigUtil::setValue(QSettings* settings, const QString& groupName, const QString& name, const QVariant& value) {
    settings->beginGroup(groupName);
    settings->setValue(name, value);
    settings->endGroup();
    settings->sync();
}
