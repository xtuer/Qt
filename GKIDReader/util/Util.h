#ifndef UTIL_H
#define UTIL_H

#include <QString>

class QNetworkReply;

class Util {
public:
    // 格式化时间
    // 把 yyyyMMdd 的时间格式化为 yyyy 年 MM 月 dd 日
    static QString formatDateCN(const QString &date);

    // 格式化时间
    // 把 yyyyMMdd 的时间格式化为 yyyy-MM-dd
    static QString formatDateISO(const QString &date);

    static QByteArray md5(const QByteArray &data);


    // 注册自定义类型
    static void registerTypes();
};

#endif // UTIL_H
