#include "MacRegister.h"

#include <QString>
#include <QSettings>
#include <QDebug>

SINGLETON_INSTANCE(MacRegister)

MacRegister::MacRegister() {
    settings = new QSettings("sn.plist", QSettings::NativeFormat);
}

MacRegister::~MacRegister() {
    delete settings;
}

int MacRegister::readUsedTimes() {
    return settings->value("used_times", 0).toInt();
}

void MacRegister::saveUsedTimes(int times) {
    settings->setValue("used_times", times);
}

QString MacRegister::readSN() {
    return settings->value("sn", "").toString();
}

void MacRegister::saveSN(const QString &sn) {
    settings->setValue("sn", sn);
}
 
