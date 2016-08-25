#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>

// [[1]] 单例的主要内容声明与实现.
// Start of SINGLETON
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
    static std::auto_ptr<Class> instance;   \

// End of SINGLETON

// [[2]] 静态instance对象的初始化.
#define SINGLETON_INSTANCE(Class) std::auto_ptr<Class> Class::instance;

// [[3]] 默认的构造函数和析构函数.
#define SINGLETON_DEFAULT_CONSTRUCTOR(Class) Class::Class() {};
#define SINGLETON_DEFAULT_DESTRUCTOR(Class) Class::~Class() {};

#endif // SINGLETON_HPP


/*
 *Singletone的实现
+-------------------------Singletone.hpp------------------------------+
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

+-------------------------Singletone.cpp------------------------------+
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
