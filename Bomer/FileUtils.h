#ifndef FILEUTILS_H
#define FILEUTILS_H

class QString;
class QByteArray;

/**
 * 读写文件的工具类
 *
 * 1. 读取文件调用 readFile()
 * 2. 写入文件调用 writeFile()
 */
class FileUtils {
public:
    // 读取文件的内容, 如果 maxSize 为 0 则读取文件的所有内容，否则读取文件中 前 maxSize 字节的数据
    static QByteArray readFile(const QString &path, int maxSize = 0);

    // 写入 content 到文件，覆盖文件原来的内容
    static void writeFile(const QString &path, const QByteArray &content);
};

#endif // FILEUTILS_H
