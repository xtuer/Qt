#include <Calculator.h>
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    // 调用库中的函数
    work();

    // 生成库中的类对象
    Calculator calculator;
    qDebug() << calculator.add(2, 3);

    return 0;
}
