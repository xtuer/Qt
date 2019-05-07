#include <QApplication>
#include <QDebug>

class B {
public:
    B() {
        qDebug() << "B()";
    }

    ~B() {
        qDebug() << "~B()";
    }

    int value = 10;
};

class A {
public:
    A() {
        qDebug() << "A()";
        b = new B();
    }

    ~A() {
        delete b;
        qDebug() << "~A()";
    }

    B *b = nullptr;
};

A a() {
    return A();
}

void foo(B *b) {
    qDebug() << b->value;
}

int main(int argc, char** argv) {
    //    QApplication app(argc, argv);

    //    return app.exec();
    foo(a().b); // 函数 foo() 执行完 a() 创建的 A 和 B 才析构，在函数 foo() 中放心的访问 b

    B *b = a().b; // 语句结束时 A 和 B 才析构，不要在后面的语句中访问 b
    qDebug() << b->value; // b 已经被析构，b->value 的值不是 10，而是随机的

    qDebug() << "main() 结束";

    return 0;
}
