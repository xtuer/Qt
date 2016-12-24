#include <QApplication>
#include <QDebug>
#include <QList>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QList<int> os;
    os.clear();

    return app.exec();
}
