#ifndef CONFIG_H
#define CONFIG_H

#define ConfigInstance Singleton<Config>::getInstance()

#include "util/Singleton.h"
#include <QVariant>

class QString;
class QStringList;

class Json;
class QSettings;

/**
 * 用于读写配置文件:
 * 1. data/config.json: 存储配置的信息，例如数据库信息，QSS 文件的路径
 * 2. 读取配置，如 ConfigInstance.getDatabaseName();
 */
class Config {
    SINGLETON(Config)

public:
    ////////////////////////////////////////////////////////////////////////////
    /// 数据库信息
    ////////////////////////////////////////////////////////////////////////////
    void destroy(); // 保存应该配置, 释放内存
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
    bool isSignInWithFace() const;               // 是否使用人脸识别登陆

    ////////////////////////////////////////////////////////////////////////////
    /// UI
    ////////////////////////////////////////////////////////////////////////////
    QStringList getQssFiles() const;  // QSS 样式表文件, 可以是多个
    QStringList getFontFiles() const; // 字体文件
    int getLayoutPadding() const; // Widget 布局的 padding
    int getLayoutSpacing() const; // Widget 中子 Widget 之间的 spacing

    void saveWindowGeometry(const QString &groupName, QWidget *window);    // 保存窗口的位置和大小
    void restoreWindowGeometry(const QString &groupName, QWidget *window); // 恢复窗口的位置和大小

    ////////////////////////////////////////////////////////////////////////////
    /// URLs
    ////////////////////////////////////////////////////////////////////////////
    QString getUrlOfExams() const;        // 考试
    QString getUrlOfUnits() const;        // 考试单元
    QString getUrlOfSites() const;        // 考点
    QString getUrlOfRooms() const;        // 考场
    QString getUrlOfEnrollments() const;  // 考生
    QString getUrlOfSignInCard() const;   // 身份签到
    QString getUrlOfSignInFace() const;   // 身份证 + 人脸识别签到
    QString getUrlOfSignInManual() const; // 手动签到
    QString getUrlOfServerTime() const;   // 服务器当前时间
    QString getUrlOfUploadPhoto() const;  // 上传摄像头照片
    QString getUrlOfClientClosePassword() const; // 关闭客户端密码

    QString getUrl(const QString &name) const; // 获取配置文件中 urls 下的 url

    // 是否调试模式
    bool isDebug() const;

private:
    QVariant getGuiValue(const  QString &groupName, const QString &name, const QVariant &def = QVariant());
    void     setGuiValue(const  QString &groupName, const QString &name, const QVariant &value);

    static QVariant getValue(QSettings *settings, const QString &groupName, const QString &name, const QVariant &def = QVariant());
    static void     setValue(QSettings *settings, const QString &groupName, const QString &name, const QVariant &value);


private:
    Json      *config; // 使用Qt的数据封装模式，数据封装在.cpp文件里，暴露给客户的信息尽量少
    QSettings *appSettings;
};

#endif // CONFIG_H
