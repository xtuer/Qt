#ifndef COMMONUTIL_H
#define COMMONUTIL_H

class CommonUtil {
public:
    static void registerMessagePattern(); // 设置 Debug 输出的格式
    static void releaseSingletonResource(); // 清理释放 Singleton 创建的资源
};

#endif // COMMONUTIL_H
