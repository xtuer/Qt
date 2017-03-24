#include <QDebug>
#include <QVariant>
#include <QByteArray>
#include <QPushButton>

class CA {
public:
    static int CS[];
};

int CA::CS[]= {
    0xC1, 0x21
};

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << CA::CS[0];

    return 0;
}
