#include "Util.h"
#include <QString>
#include <Qdate>
#include <QNetworkReply>
#include <QTextStream>
#include <QCryptographicHash>

#include "bean/Person.h"

QString Util::formatDate(const QString &date) {
    return QDate::fromString(date, "yyyyMMdd").toString("yyyy 年 MM 月 dd 日");
}

void Util::registerTypes() {
    qRegisterMetaType<Person>("Person");
    qRegisterMetaType<Person>("Person&");
}

QByteArray Util::md5(const QByteArray &data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}
