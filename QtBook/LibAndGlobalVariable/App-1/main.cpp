#include <Lib-1/Config.h>
#include <QDebug>
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton *button = new QPushButton("App-1: Access Global Variable");
    button->show();

    QObject::connect(button, &QPushButton::clicked, [button] {
        qDebug() << &Config::instance();
        ++Config::count;
        ++g;
        button->setText(QString("App-1: count %1, g %2").arg(Config::count).arg(g));
    });

    return app.exec();
}
