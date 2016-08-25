#include <QString>
#include <QDebug>

struct Flag {
    QString hello;
};

struct Outter {
    Flag flag;
};

int main(int argc, char *argv[]) {
    Outter outter;
    outter.flag.hello = "Hi";

    qDebug() << outter.flag.hello;

   return 0;
}
