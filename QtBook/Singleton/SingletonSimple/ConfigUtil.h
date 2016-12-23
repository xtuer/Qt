#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include <QMutex>
#include <QString>

class Config {
public:
    // 这个函数和单例的实现无关，是 ConfigUtil 的功能函数
    QString getDatabaseName() const;

    /**
     * @brief 获取 ConfigUtil 的唯一对象
     * @return ConfigUtil 的对象
     */
    static Config& getInstance();

    /**
     * @brief 删除 ConfigUtil 的唯一对象
     */
    static void release();

private:
    Config();  // 构造函数
    ~Config(); // 析构函数
    Config(const Config &other); // 拷贝构造函数
    Config& operator=(const Config &other); // 赋值运算操作符

    static QMutex mutex;
    static Config *instance; // ConfigUtil 全局唯一的变量
};

#endif // CONFIGUTIL_H
