#include "Register.h"
#include "../util/SNUtil.h"
#include <qDebug>

/**
 * 生成与验证序列号.
 * 生成序列号:
 *     1. 用如mac地址生成机器码.
 *     2. 用机器码生成序列号.
 * 验证序列号:
 *     注册时用机器码生成序列号与客户提供的序列号比较, 如相等则是合法的序列号.
 */
Register::Register() { }

Register::~Register() { }

void Register::usedAgain() {
    int currentUsedTimes = readUsedTimes() + 1;
    saveUsedTimes(currentUsedTimes);
}

bool Register::isSNValid(const QString &sn, const QString &machineCode) {
    return SNUtil::generateSN(machineCode) == sn;
}

QString Register::getMachineCode() {
    QString code = SNUtil::getMacAddress().mid(0, 16);

    return SNUtil::generateSN(code);
}

QString Register::getRegisterInfomation() {
    // [[1]] 已注册
    if (isRegistered()) {
        return QString("已注册");
    }

    // [[2]] 未注册，已过期, 返回false，不可使用
    if (isExpired()) {
        return QString("试用期已过, 请支持正版");
    }

    // [[3]] 未注册，未过期, 返回true, 可以使用
    int times = readUsedTimes();
    return QString("您还可以使用 %1 次").arg(Max_Time - times);
}

bool Register::isRegistered() {
    QString sn = readSN();
    QString mc = getMachineCode();

    return isSNValid(sn, mc);
}

bool Register::isExpired() {
    int times = readUsedTimes();

    return !isRegistered() && (times > Max_Time);
}

bool Register::registerSN(const QString &sn) {
    QString mc = getMachineCode();

    if (isSNValid(sn, mc)) {
        saveSN(sn);

        return true;
    }

    return false;
}
