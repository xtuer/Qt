#ifndef UTIL_H
#define UTIL_H

#include <QString>

class QNetworkReply;

class Util {
public:
    // 格式化时间
    // 把 yyyyMMdd 的时间格式化为 yyyy 年 MM 月 dd 日
    static QString formatDate(const QString &date);

    // 注册自定义类型
    static void registerTypes();

    static QByteArray md5(const QByteArray &data);
};

#endif // UTIL_H
