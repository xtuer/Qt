#ifndef PANDORA_H
#define PANDORA_H
#include <QString>
#include <QDebug>

class Pandora {
public:
    Pandora() {
        age = 10;
    }

    int age;
};

extern "C" {
void foo();
}
#endif // PANDORA_H
