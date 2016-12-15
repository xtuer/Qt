#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

// [1] 单例的主要内容声明与实现.
// Start of SINGLETON Declaration, Class 是类名
#define SINGLETON(Class)                        \
public:                                         \
    static Class& getInstance() {               \
        if (instance.isNull()) {                \
            mutex.lock();                       \
            if (instance.isNull()) {            \
                instance.reset(new Class());    \
            }                                   \
            mutex.unlock();                     \
        }                                       \
                                                \
        return *instance.data();                \
    }                                           \
                                                \
private:                                        \
    Class();                                    \
    ~Class();                                   \
    Class(const Class &other);                  \
    Class& operator=(const Class &other);       \
    static QMutex mutex;                        \
    static QScopedPointer<Class> instance;      \
    friend class QScopedPointer<Class>;         \
    friend struct QScopedPointerDeleter<Class>;

// End of SINGLETON Declaration

// [2] 静态对象的初始化
#define SINGLETON_STATIC_INITIALIZE(Class)      \
    QMutex Class::mutex;                        \
    QScopedPointer<Class> Class::instance;

#endif // SINGLETON_H
