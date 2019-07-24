#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

#if defined(GOO)
    qDebug() << "YES";
#else
    qDebug() << "No";
#endif

    return app.exec();
}
