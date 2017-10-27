#include <QApplication>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString str;
    qDebug() << str.isEmpty();

    return 0;
}
