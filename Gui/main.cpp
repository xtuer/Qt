#include <QApplication>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug().noquote() << QString("Greeting: %1").arg("Biao");

    return 0;
}
