#include "ConfigUtil.h"
#include <QDebug>

QMutex ConfigUtil::mutex;
QScopedPointer<ConfigUtil> ConfigUtil::instance;

ConfigUtil::ConfigUtil() {
    qDebug() << "ConfigUtil()";
}

ConfigUtil::~ConfigUtil() {
    qDebug() << "~ConfigUtil()";
}

QString ConfigUtil::getDatabaseName() const {
    return "Verbose";
}

ConfigUtil& ConfigUtil::getInstance() {
    if (instance.isNull()) {
        mutex.lock();
        if (instance.isNull()) {
            instance.reset(new ConfigUtil());
        }
        mutex.unlock();
    }

    return *instance.data();
}
