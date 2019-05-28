#include "Config.h"
#include "Json.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QWidget>

Config::Config() {
    config      = new Json("data/config.json", true); // 配置文件路径
    appSettings = new QSettings("data/app.ini", QSettings::IniFormat);
    appSettings->setIniCodec("UTF-8");
}

Config::~Config() {

}

void Config::destroy() {
    delete config;
    config = NULL;

    if (NULL != appSettings) {
        appSettings->sync(); // 保存配置
    }

    delete appSettings;
    appSettings = NULL;
}

QString Config::getDatabaseType() const {
    return config->getString("database.type");
}

QString Config::getDatabaseHost() const {
    return config->getString("database.host");
}

QString Config::getDatabaseName() const {
    return config->getString("database.database_name");
}

QString Config::getDatabaseUsername() const {
    return config->getString("database.username");
}

QString Config::getDatabasePassword() const {
    return config->getString("database.password");
}

bool Config::getDatabaseTestOnBorrow() const {
    return config->getBool("database.test_on_borrow", false);
}

QString Config::getDatabaseTestOnBorrowSql() const {
    return config->getString("database.test_on_borrow_sql", "SELECT 1");
}

int Config::getDatabaseMaxWaitTime() const {
    return config->getInt("database.max_wait_time", 5000);
}

int Config::getDatabaseMaxConnectionCount() const {
    return config->getInt("database.max_connection_count", 5);
}

int Config::getDatabasePort() const {
    return config->getInt("database.port", 0);
}

bool Config::isDatabaseDebug() const {
    return config->getBool("database.debug", false);
}

QStringList Config::getDatabaseSqlFiles() const {
    return config->getStringList("database.sql_files");
}

bool Config::isSignInWithFace() const {
    return config->getBool("signInWithFace", false);
}

QStringList Config::getQssFiles() const {
    return config->getStringList("qss_files");
}

QStringList Config::getFontFiles() const {
    return config->getStringList("font_files");
}

int Config::getLayoutPadding() const {
    return config->getInt("ui.padding", 10);
}

int Config::getLayoutSpacing() const {
    return config->getInt("ui.spacing", 5);
}

void Config::saveWindowGeometry(const QString &groupName, QWidget *window) {
    setGuiValue(groupName, "pos", window->pos());
    setGuiValue(groupName, "size", window->rect().size());
}

void Config::restoreWindowGeometry(const QString &groupName, QWidget *window) {
    window->move(getGuiValue(groupName, "pos", QPoint(0, 0)).toPoint());
    window->resize(getGuiValue(groupName, "size", QSize(600, 400)).toSize());
}

QString Config::getServerUrl() const {
    return config->getString("serverUrl");
}

// 是否调试模式
bool Config::isDebug() const {
    return config->getBool("debug", false);
}

QVariant Config::getGuiValue(const QString& groupName, const QString& name, const QVariant& def) {
    return getValue(appSettings, groupName, name, def);
}

void Config::setGuiValue(const QString& groupName, const QString& name, const QVariant& value) {
    setValue(appSettings, groupName, name, value);
}

QVariant Config::getValue(QSettings* settings, const QString& groupName, const QString& settingName, const QVariant &def) {
    QVariant var;

    settings->beginGroup(groupName);
    var = settings->value(settingName, def);
    settings->endGroup();

    return var;
}

void Config::setValue(QSettings* settings, const QString& groupName, const QString& name, const QVariant& value) {
    settings->beginGroup(groupName);
    settings->setValue(name, value);
    settings->endGroup();
    settings->sync();
}
