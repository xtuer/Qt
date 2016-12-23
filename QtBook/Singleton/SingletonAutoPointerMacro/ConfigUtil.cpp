#include "ConfigUtil.h"

SINGLETON_STATIC_INITIALIZE(ConfigUtil) // [3]

#include <QDebug>

Config::Config() {
    qDebug() << "ConfigUtil()";
}

Config::~Config() {
    qDebug() << "~ConfigUtil()";
}

QString Config::getDatabaseName() const {
    return "Pandora";
}
