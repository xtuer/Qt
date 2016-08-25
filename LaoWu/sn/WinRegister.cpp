#include "WinRegister.h"
#include "../util/SNUtil.h"

#include <QString>
#include <QSettings>

SINGLETON_INSTANCE(WinRegister)

WinRegister::WinRegister() {
    snSettings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Shell",
                               QSettings::NativeFormat);

    usedTimesSettings = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Speech",
                                      QSettings::NativeFormat);
}

WinRegister::~WinRegister() {
    delete snSettings;
    delete usedTimesSettings;
}

int WinRegister::readUsedTimes() {
    return usedTimesSettings->value("used_times", 0).toInt();
}

void WinRegister::saveUsedTimes(int times) {
    usedTimesSettings->setValue("used_times", times);
}

QString WinRegister::readSN() {
    return snSettings->value("sn", "").toString();
}

void WinRegister::saveSN(const QString &sn) {
    snSettings->setValue("sn", sn);
}

/**
 * 取得机器的相关信息计算注册码，如果没有网卡，会再使用计算的CPU信息来计算注册码
 */
QString WinRegister::getMachineCode() {
    QString macAddress = SNUtil::getMacAddress();
    QString otherHardwareInfo;

    QString regPath = "HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
    QSettings reg(regPath, QSettings::NativeFormat);
    otherHardwareInfo = QString::number(qAbs(reg.value("FeatureSet", 0).toLongLong()));

    // 使用16位的注册码， 多了也没必要
    return SNUtil::generateSN((macAddress + otherHardwareInfo).mid(0, 16));
}
 
