#include "ConfigUtil.h"
#include <QDebug>

QMutex Config::mutex;
QScopedPointer<Config> Config::instance;

Config::Config() {
    qDebug() << "ConfigUtil()";
}

Config::~Config() {
    qDebug() << "~ConfigUtil()";
}

QString Config::getDatabaseName() const {
    return "Verbose";
}

Config& Config::getInstance() {
    if (instance.isNull()) {
        mutex.lock();
        if (instance.isNull()) {
            instance.reset(new Config());
        }
        mutex.unlock();
    }

    return *instance.data();
}
