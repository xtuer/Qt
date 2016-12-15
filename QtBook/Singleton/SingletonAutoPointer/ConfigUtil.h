#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include <QString>
#include <QMutex>
#include <QScopedPointer>

class ConfigUtil {
public:
    QString getDatabaseName() const;

    static ConfigUtil& getInstance();
    static void release();

private:
    ConfigUtil();
    ~ConfigUtil();
    ConfigUtil(const ConfigUtil &other);
    ConfigUtil& operator=(const ConfigUtil &other);

    static QMutex mutex;
    static QScopedPointer<ConfigUtil> instance;
    friend struct QScopedPointerDeleter<ConfigUtil>;
};

#endif // CONFIGUTIL_H
