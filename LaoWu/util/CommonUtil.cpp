#include "CommonUtil.h"

#include <QDebug>
#include <QCryptographicHash>

const QChar src[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z'};

const QChar dst[] = {
    'A', 'U', 'L', 't', 'K', 'V', 'j', 'z', 's', 'r',
    'O', 'N', 'b', 'S', 'Z', 'c', 'd', 'h', 'y', 'a',
    'f', 'x', 'Y', 'B', 'l', 'o', 'C', 'M', 'R', 'E',
    'Q', 'q', 'W', 'u', 'J', 'H', 'e', 'X', 'F', 'P',
    'm', 'p', 'D', 'w', 'G', 'v', 'I', 'i', 'g', 'k',
    'n', 'T'};

const QString CommonUtil::Date_Format("yyyy-MM-dd");
const QString CommonUtil::DateTime_Format("yyyy-MM-dd hh:mm:ss");

QString CommonUtil::dateToString(const QDate &date) {
    return date.toString(Date_Format);
}

QString CommonUtil::dateToString(const QDateTime &dateTime) {
    return dateTime.toString(Date_Format);
}

QString CommonUtil::dateTimeToString(const QDate &date) {
    return date.toString(DateTime_Format);
}

QString CommonUtil::dateTimeToString(const QDateTime &dateTime) {
    return dateTime.toString(DateTime_Format);
}

QDateTime CommonUtil::dateTimeFromString(const QString &str) {
    return QDateTime::fromString(str, DateTime_Format);
}
 
QString CommonUtil::md5(const QString string) {
    return QString(QCryptographicHash::hash(string.toLocal8Bit(), QCryptographicHash::Md5).toHex().toUpper());
}

int CommonUtil::boolToInt(bool b) {
    return b ? 1 : 0;
}

int CommonUtil::intToBool(int n) {
    return n == 0 ? false : true;
}

QString CommonUtil::encodeString(const QString &str) {
    QString ret = toBase64(str);
    encode(ret);

    return ret;
}

QString CommonUtil::decodeString(const QString &str) {
    QString ret(str);
    decode(ret);
    ret = fromBase64(ret);

    return ret;
}

QString CommonUtil::encodeFloat(float value) {
    return encodeString(QString::number(value));
}

float CommonUtil::decodeFloat(const QString &str) {
    return decodeString(str).toFloat();
}

QString CommonUtil::toBase64(const QString &str) {
    return str.toUtf8().toHex().toBase64();
}

QString CommonUtil::fromBase64(const QString &base64) {
    QByteArray ba;
    ba.append(base64);

    return QString::fromUtf8(QByteArray::fromHex(QByteArray::fromBase64(ba)));
}

QString& CommonUtil::encode(QString &str) {
    for (int i = 0; i < str.size(); ++i) {
        for (int j = 0; j < 52; ++j) {
            if (src[j] == str.at(i)) {
                str.replace(i, 1, dst[j]);
                break;
            }
        }
    }

    return str;
}

QString& CommonUtil::decode(QString &str) {
    for (int i = 0; i < str.size(); ++i) {
        for (int j = 0; j < 52; ++j) {
            if (dst[j] == str.at(i)) {
                str.replace(i, 1, src[j]);
                break;
            }
        }
    }

    return str;
}
