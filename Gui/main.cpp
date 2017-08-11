#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QString str("Hello 中国");
    std::string foo = str.toStdString();
    std::cout << foo << std::endl;

    return a.exec();
}
