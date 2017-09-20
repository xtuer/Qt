#include <QApplication>
#include <QPixmap>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QPixmap p("/Users/Biao/Desktop/x.png");
    p.save("/Users/Biao/Desktop/y.png");

    return a.exec();
}
