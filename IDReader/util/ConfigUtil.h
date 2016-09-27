#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include "util/Singleton.h"
#include <QVariant>

class QString;
class QStringList;

class JsonReader;
class ScheduleDescription;
class QSettings;

/**
 * 用于读写配置文件:
 * 1. data/config.json: 存储配置的信息，例如数据库信息，QSS 文件的路径
 */
class ConfigUtil {
    SINGLETON(ConfigUtil)

public:
    ////////////////////////////////////////////////////////////////////////////
    /// 数据库信息
    ////////////////////////////////////////////////////////////////////////////
    void release();
    QString getDatabaseType() const;             // 数据库的类型, 如QPSQL, QSQLITE, QMYSQL
    QString getDatabaseHost() const;             // 数据库主机的IP
    QString getDatabaseName() const;             // 数据库名
    QString getDatabaseUsername() const;         // 登录数据库的用户名
    QString getDatabasePassword() const;         // 登录数据库的密码
    QString getDatabaseTestOnBorrowSql() const;  // 验证连接的 SQL
    bool getDatabaseTestOnBorrow() const;        // 是否验证连接
    int  getDatabaseMaxWaitTime() const;         // 线程获取连接最大等待时间
    int  getDatabaseMaxConnectionCount() const;  // 最大连接数
    int  getDatabasePort() const;                // 数据库的端口号
    bool isDatabaseDebug() const;                // 是否打印出执行的 SQL 语句和参数
    QStringList getDatabaseSqlFiles() const;     // SQL 语句文件, 可以是多个

    ////////////////////////////////////////////////////////////////////////////
    /// UI
    ////////////////////////////////////////////////////////////////////////////
    QStringList getQssFiles() const; // QSS 样式表文件, 可以是多个
    int getLayoutPadding() const; // Widget 布局的 padding
    int getLayoutSpacing() const; // Widget 中子 Widget 之间的 spacing

    void saveWindowGeometry(const QString &groupName, QWidget *window);    // 保存窗口的位置和大小
    void restoreWindowGeometry(const QString &groupName, QWidget *window); // 恢复窗口的位置和大小

    ////////////////////////////////////////////////////////////////////////////
    /// 其它
    ////////////////////////////////////////////////////////////////////////////
    QString getServerUrl() const;

private:
    QVariant getGuiValue(const  QString &groupName, const QString &name, const QVariant &def = QVariant());
    void     setGuiValue(const  QString &groupName, const QString &name, const QVariant &value);

    static QVariant getValue(QSettings *settings, const QString &groupName, const QString &name, const QVariant &def = QVariant());
    static void     setValue(QSettings *settings, const QString &groupName, const QString &name, const QVariant &value);


private:
    JsonReader *config; // 使用Qt的数据封装模式，数据封装在.cpp文件里，暴露给客户的信息尽量少
    QSettings  *guiSettings;
};

#endif // CONFIGUTIL_H
