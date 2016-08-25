#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include <QString>
#include <QList>
#include <QDate>
#include <QDateTime>

class CommonUtil {
public:
    template<typename T>
    static void deleteAll(QList<T *> *list) {
        qDeleteAll(list->begin(), list->end());
        list->clear();
    }

    static QString dateToString(const QDate &date);
    static QString dateToString(const QDateTime &dateTime);
    static QString dateTimeToString(const QDate &date);
    static QString dateTimeToString(const QDateTime &dateTime);

    static QDateTime dateTimeFromString(const QString &str);

    static QString md5(const QString string);

    static int boolToInt(bool b);
    static int intToBool(int n);

    static QString encodeString(const QString &str);
    static QString decodeString(const QString &str);

    static QString encodeFloat(float value);
    static float   decodeFloat(const QString &str);

private:
    CommonUtil() {}
    CommonUtil(const CommonUtil &);
    CommonUtil& operator=(const CommonUtil &);

    static QString toBase64(const QString &str);
    static QString fromBase64(const QString &str);

    static QString& encode(QString &str);
    static QString& decode(QString &str);

    static const QString Date_Format;
    static const QString DateTime_Format;
};

#endif // COMMONUTIL_H
