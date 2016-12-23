#include "ConfigUtil.h"
#include <QDebug>

QMutex Config::mutex;
Config* Config::instance = 0;

Config::Config() {
    qDebug() << "ConfigUtil()";
}

Config::~Config() {
    qDebug() << "~ConfigUtil()";
}

Config& Config::getInstance() {
    if (0 == instance) {
        mutex.lock();
        if (0 == instance) {
            instance = new Config();
        }
        mutex.unlock();
    }

    return *instance;
}

void Config::release() {
    if (0 != instance) {
        mutex.lock();
        delete instance;
        instance = 0;
        mutex.unlock();
    }
}

QString Config::getDatabaseName() const {
    return "Avatar";
}
