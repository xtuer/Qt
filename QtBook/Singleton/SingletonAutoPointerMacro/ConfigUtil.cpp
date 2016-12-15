#include "ConfigUtil.h"

SINGLETON_STATIC_INITIALIZE(ConfigUtil) // [3]

#include <QDebug>

ConfigUtil::ConfigUtil() {
    qDebug() << "ConfigUtil()";
}

ConfigUtil::~ConfigUtil() {
    qDebug() << "~ConfigUtil()";
}

QString ConfigUtil::getDatabaseName() const {
    return "Pandora";
}
