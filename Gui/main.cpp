#include <QApplication>
#include <QDebug>
#include "Widget.h"

#include <QList>
#include <algorithm>
#include <QTime>

// sort using a custom function object
struct {
    bool operator()(int a, int b) const
    {
        return a > b;
    }
} customGreater;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const int count = 10000000;
    QTime time;

    // 非 Lambda 排序
    time.start();
    QList<int> ns2 = QList<int>() << 1 << 3 << 5 << 4 << 2 << 23 << 123 << 2342;
    for (int i = 0; i < count; ++i) {
        std::sort(ns2.begin(), ns2.end(), customGreater);
    }
    qDebug() << time.elapsed();

    // Lambda 排序
    time.restart();
    QList<int> ns1 = QList<int>() << 1 << 3 << 5 << 4 << 2 << 23 << 123 << 2342;
    for (int i = 0; i < count; ++i) {
        std::sort(ns1.begin(), ns1.end(), [](int a, int b) -> bool {
            return a > b;
        });
    }
    qDebug() << time.elapsed();



    return 0;
}
