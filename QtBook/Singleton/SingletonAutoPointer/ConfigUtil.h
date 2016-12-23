#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include <QString>
#include <QMutex>
#include <QScopedPointer>

class Config {
public:
    QString getDatabaseName() const;

    static Config& getInstance();
    static void release();

private:
    Config();
    ~Config();
    Config(const Config &other);
    Config& operator=(const Config &other);

    static QMutex mutex;
    static QScopedPointer<Config> instance;
    friend struct QScopedPointerDeleter<Config>;
};

#endif // CONFIGUTIL_H
