#include <Lib-1/Config.h>
#include <Lib-2/Form.h>

#include <QDebug>
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton *button = new QPushButton("App-2: Access Global Variable");
    button->show();

    QObject::connect(button, &QPushButton::clicked, [button] {
        qDebug() << &Config::instance();
        ++Config::count;
        ++g;
        button->setText(QString("App-2: count %1, g %2").arg(Config::count).arg(g));
    });

    Form form;
    form.show();

    return app.exec();
}
