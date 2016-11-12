#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                            Singleton signature                           ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////
/**
 * 类模版 Singleton 用于辅助实现单例模式。
 * 使用方法，以定义类 ConnectionPool 的单例实现为例:
 * 0. 包含单例的头文件:
 *    #include "Singleton.h"
 *
 * 1. 声明类要实现单例:
 *    class ConnectionPool {
 *        SINGLETON(ConnectionPool) // Here
 *    public:
 *
 * 2. 实现单例类默认的构造函数和析构函数
 *    ConnectionPool::ConnectionPool() {};
 *    ConnectionPool::~ConnectionPool() {};
 *
 * 3. 获取单例类的对象，就可以调用它的函数了:
 *    QSqlDatabase connection = Singleton<ConnectionPool>::getInstance().getConnection();
 *
 *    ConnectionPool &pool = Singleton<ConnectionPool>::getInstance();
 *    QSqlDatabase connection = pool.getConnection();
 *
 * 注意: 如果单例的类需要释放的资源和 Qt 底层的信号系统有关系，例如 QSettings，QSqlDatabase 等，
 *    需要在程序结束前手动释放(也就是在 main() 函数返回前调用释放资源的函数)，
 *    否则有可能在程序退出时报系统底层的信号错误，导致如 QSettings 的数据没有保存。
 */
template <typename T>
class Singleton {
public:
    static T& getInstance();

    Singleton(const Singleton &other);
    Singleton<T>& operator=(const Singleton &other);

private:
    static QMutex mutex;
    static QScopedPointer<T> instance;
};

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                            Singleton definition                          ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////
template <typename T> QMutex Singleton<T>::mutex;
template <typename T> QScopedPointer<T> Singleton<T>::instance;

template <typename T>
T& Singleton<T>::getInstance() {
    if (instance.isNull()) {
        mutex.lock();
        if (instance.isNull()) {
            instance.reset(new T());
        }
        mutex.unlock();
    }

    return *instance.data();
}

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                               Singleton Macro                            ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////
#define SINGLETON(Class)                        \
private:                                        \
    Class();                                    \
    ~Class();                                   \
    Class(const Class &other);                  \
    Class& operator=(const Class &other);       \
    friend class Singleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;

#endif // SINGLETON_H
