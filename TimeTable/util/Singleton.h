#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

// [[1]] 单例的主要内容声明与实现.
#define SINGLETON(Class)                    \
public:                                     \
    static Class& getInstance() {           \
        if (instance.get() == 0) {          \
            instance.reset(new Class());    \
        }                                   \
                                            \
        return *(instance.get());           \
    }                                       \
                                            \
private:                                    \
    Class();                                \
    ~Class();                               \
    Class(const Class &);                   \
    Class& operator=(const Class &);        \
    friend class std::auto_ptr<Class>;      \
    static std::auto_ptr<Class> instance;

// [[2]] 静态instance对象的初始化.
#define SINGLETON_INITIALATE(Class) std::auto_ptr<Class> Class::instance;

// [[3]] 默认的构造函数和析构函数, 有些情况下我们需要提供自己的构造函数和析构函数.
#define SINGLETON_CONSTRUCTOR(Class) Class::Class() {}
#define SINGLETON_DESTRUCTOR(Class) Class::~Class() {}

// [[2-3默认实现全放一起]]
#define SINGLETON_ALL_IN_ONE(Class)      \
    std::auto_ptr<Class> Class::instance;\
    Class::Class() {}                    \
    Class::~Class() {}

#endif // SINGLETON_HPP

/*
SINGLETON 的使用
+-------------------------Util.h------------------------------+
class Util {
    SINGLETON(Util)
public:
    fun();
}

+-------------------------Util.cpp------------------------------+
SINGLETON_INITIALATE(Util)
SINGLETON_CONSTRUCTOR(Util)
SINGLETON_DESTRUCTOR(Util)

+-------------------------or Util.cpp------------------------------+
SINGLETON_ALL_IN_ONE(Util)
*/

/**
SINGLETON 的实现代码
+-------------------------Singleton.hpp------------------------------+
#include <memory>
class Singleton {
public:
    static Singleton& getInstance();

private:
    Singleton();
    ~Singleton();
    Singleton(const Singleton &);
    Singleton& operator=(const Singleton &);

    friend class std::auto_ptr<Singleton>;
    static std::auto_ptr<Singleton> instance;
};

+-------------------------Singleton.cpp------------------------------+
std::auto_ptr<Singleton> Singleton::instance;

Singleton::Singleton() {
}

Singleton::~Singleton() {
    qDebug() << QString("Singleton::~Singleton()");
}

Singleton& Singleton::getInstance() {
   if (instance.get() == 0) {
        instance.reset(new Singleton());
    }

    return *(instance.get());
}
*/
