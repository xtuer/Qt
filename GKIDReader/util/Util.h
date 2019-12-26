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

    /**
     * 把图片转为 Base64 的字符串
     *
     * @param imagePath 图片文件的路径
     * @param error 错误标志，转换失败则 *error 为 true，否则为 false
     * @return 返回图片的 base64 的字符串表示
     */
    static QString imageToBase64String(const QString &imagePath, bool *error = nullptr);
};

#endif // UTIL_H
