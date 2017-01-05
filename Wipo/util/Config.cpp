#include "Config.h"
#include <QCoreApplication>

QString Config::getQssFilePath() {
    return QCoreApplication::applicationDirPath() + "/resources/style.qss";
}

QString Config::getConfigFilePath() {
    return QCoreApplication::applicationDirPath() + "/data/config.json";
}
