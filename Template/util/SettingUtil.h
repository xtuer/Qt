#ifndef SETTINGUTIL_H
#define SETTINGUTIL_H

#include "util/Singleton.h"
#include <QVariant>

class QString;
class QStringList;
class QSettings;
class QSplitter;
class QWidget;

class SettingUtilPrivate;

class SettingUtil {
    // 使用单例模式:
    // 取得对象 SettingUtil::getInstance()
    // 删除对象 SettingUtil::release()
    SINGLETON(SettingUtil)

public:
    // 数据库信息
    QString getDatabaseType() const; // 数据库的类型, 如QPSQL, QSQLITE, QMYSQL
    QString getDatabaseHost() const; // 数据库主机的IP
    QString getDatabaseName() const; // 数据库名
    QString getDatabaseUsername() const; // 登录数据库的用户名
    QString getDatabasePassword() const; // 登录数据库的密码
    QStringList getSqlFileNames() const; // Sql语句文件, 可以是多个.
    bool    isSqlDebug() const; // 是否打印出执行的 SQL 语句

    QStringList getQssFileNames() const; // QSS样式表文件, 可以是多个.

    // UI settings
    void saveSplitterState(const QString &groupName, QSplitter *splitter);          // 保存splitter的状态
    void restoreSplitterState(const QString &groupName, QSplitter *splitter) const; // 恢复splitter的状态
    void saveWindowGeometry(const QString &groupName, QWidget *window);             // 保存窗口的位置与大小
    void restoreWindowGeometry(const QString &groupName, QWidget *window) const;    // 恢复窗口的位置与大小

private:
    SettingUtilPrivate *data; // 使用Qt的数据封装模式，数据封装在.cpp文件里，暴露给客户的信息尽量少
};

#endif // SETTINGUTIL_H
