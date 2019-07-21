#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton *b = new QPushButton("Button");
    b->show();

    // 点击按钮后它会隐藏 2 秒，然后移动到 (300, 200)，再然后才显示吗？
    QObject::connect(b, &QPushButton::clicked, [&] {
        b->hide();
        QThread::sleep(2);

        b->move(300, 200);
        QThread::sleep(2);

        b->show();
    });

    return app.exec();
}
