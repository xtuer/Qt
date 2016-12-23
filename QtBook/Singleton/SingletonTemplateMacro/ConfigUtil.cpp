#include "ConfigUtil.h"
#include <QDebug>

Config::Config() {
    qDebug() << "ConfigUtil()";
}

Config::~Config() {
    qDebug() << "~ConfigUtil()";
}

QString Config::getDatabaseName() const {
    return "Pirate";
}
