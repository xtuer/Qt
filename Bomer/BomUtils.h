#ifndef BOMUTILS_H
#define BOMUTILS_H

class QString;

/**
 * 添加删除文件中 BOM 的工具
 *
 * 1. 添加 BOM 到文件调用 insertBom()
 * 2. 删除 BOM 调用 removeBom()
 * 3. 查看文件是否有 BOM 调用 hasBom()
 */
class BomUtils {
public:
    // 判断文件是否含有 BOM
    static bool hasBom(const QString &path);

    // 给文件添加 BOM
    static void insertBom(const QString &path);

    // 删除文件中的 BOM
    static void removeBom(const QString &path);
};

#endif // BOMUTILS_H
