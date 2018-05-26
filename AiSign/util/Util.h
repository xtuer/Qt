#ifndef UTIL_H
#define UTIL_H

class QFile;
class QString;
class QByteArray;

class Util {
public:
    /**
     * 保存字符串 content 到 path 对应的文件，编码使用 UTF-8
     *
     * @param content 要保存的字符串
     * @param path    要保存的文件路径
     * @param error   错误信息
     * @return 保存成功返回 true，保存失败返回 false
     */
    static bool writeStringToFile(const QString &content, const QString &path, QString *error = 0);

    // 格式化时间
    // 把 yyyyMMdd 的时间格式化为 yyyy 年 MM 月 dd 日
    static QString formatDate(const QString &date);

    // 注册自定义类型
    static void registerTypes();

    static QByteArray md5(const QByteArray &data);
};

#endif // UTIL_H
