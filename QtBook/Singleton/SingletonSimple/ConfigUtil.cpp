#include "ConfigUtil.h"
#include <QDebug>

QMutex ConfigUtil::mutex;
ConfigUtil* ConfigUtil::instance = 0;

ConfigUtil::ConfigUtil() {
    qDebug() << "ConfigUtil()";
}

ConfigUtil::~ConfigUtil() {
    qDebug() << "~ConfigUtil()";
}

ConfigUtil& ConfigUtil::getInstance() {
    if (0 == instance) {
        mutex.lock();
        if (0 == instance) {
            instance = new ConfigUtil();
        }
        mutex.unlock();
    }

    return *instance;
}

void ConfigUtil::release() {
    if (0 != instance) {
        mutex.lock();
        delete instance;
        instance = 0;
        mutex.unlock();
    }
}

QString ConfigUtil::getDatabaseName() const {
    return "Avatar";
}
