#include "Util.h"
#include <QString>
#include <QDate>
#include <QNetworkReply>
#include <QTextStream>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>

#include "bean/Person.h"

QString Util::formatDateCN(const QString &date) {
    return QDate::fromString(date, "yyyyMMdd").toString("yyyy 年 MM 月 dd 日");
}

QString Util::formatDateISO(const QString &date) {
    return QDate::fromString(date, "yyyyMMdd").toString("yyyy-MM-dd");
}

QByteArray Util::md5(const QByteArray &data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}

void Util::registerTypes() {
    qRegisterMetaType<Person>("Person");
    qRegisterMetaType<Person>("Person&");
}

// 把图片转为 Base64 的字符串
QString Util::imageToBase64String(const QString &imagePath, bool *error) {
    if (nullptr != error) {
        *error = false;
    }

    QFile img(imagePath);

    if (!img.open(QIODevice::ReadOnly)) {
        if (nullptr != error) {
            *error = true;
        }

        qDebug() << "打开文件出错";
        return "";
    }

    QString ext = QFileInfo(imagePath).suffix().toLower();
    QString content = img.readAll().toBase64();
    return QString("data:image/%1;base64,%2").arg(ext).arg(content);
}
