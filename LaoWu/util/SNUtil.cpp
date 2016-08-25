#include "SNUtil.h"
#include <QString>
#include <QNetworkInterface>

QString SNUtil::encryptSN(const QString &sn) {
    QString cipher;
    const int maskLength = 4;
    int mask[maskLength] = {7, 13, 23, 37};

    for(int i = 0; i < sn.size(); ++i) {
        cipher += QChar((sn.at(i).unicode() * mask[i % maskLength]) % 26 + QChar('A').unicode());
    }

    return cipher;
}

QString SNUtil::formatSN(const QString &sn) {
    QString formattedSN;

    for (int i = 0; i < sn.size(); ++i) {
        if ((i != 0) && (i % 4 == 0)) {
            formattedSN.append("-");
        }

        formattedSN.append(sn.at(i));
    }

    return formattedSN;
}

QString SNUtil::generateSN(const QString &machineCode) {
    QString code(machineCode.toUpper());
    code = code.replace(QString("-"), QString());

    return SNUtil::formatSN(SNUtil::encryptSN(code));
}

QString SNUtil::getMacAddress() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    if (interfaces.size() != 0) {
        foreach (QNetworkInterface i, interfaces) {
            if (!i.hardwareAddress().isEmpty()) {
                return i.hardwareAddress();
            }
        }
    }

    return QString();
}
