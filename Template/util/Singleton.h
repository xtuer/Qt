#ifndef SINGLETON_H
#define SINGLETON_H

// [[1]] 单例的主要内容声明与实现.
#define SINGLETON(Class)                \
public:                                 \
    static Class& getInstance() {       \
        if (0 == instance) {            \
            instance = new Class();     \
        }                               \
                                        \
        return *instance;               \
    }                                   \
                                        \
    static void release() {             \
        delete instance;                \
        instance = 0;                   \
    }                                   \
                                        \
private:                                \
    Class();                            \
    ~Class();                           \
    Class(const Class &);               \
    Class& operator=(const Class &);    \
    static Class *instance;

// [[2]] 静态instance对象的初始化.
#define SINGLETON_INITIALIZE(Class) Class* Class::instance = 0;

// [[3]] 默认的构造函数和析构函数, 有些情况下我们需要提供自己的构造函数和析构函数.
#define SINGLETON_CONSTRUCTOR(Class) Class::Class() {}
#define SINGLETON_DESTRUCTOR(Class) Class::~Class() {}

// [[2-3 默认实现全放一起]]
#define SINGLETON_ALL_IN_ONE(Class) \
Class* Class::instance = NULL;      \
Class::Class() {}                   \
Class::~Class() {}

#endif // SINGLETON_H

/*
SINGLETON 的使用
+-------------------------Util.h--------------------------------+
class Util {
    SINGLETON(Util)
public:
    fun();
}

+-------------------------Util.cpp------------------------------+
SINGLETON_INITIALIZE(Util)
SINGLETON_CONSTRUCTOR(Util)
SINGLETON_DESTRUCTOR(Util)

+-------------------------or Util.cpp---------------------------+
SINGLETON_ALL_IN_ONE(Util)

+-------------------------main.cpp------------------------------+
Calls Util::release() to release the memeory.
*/
