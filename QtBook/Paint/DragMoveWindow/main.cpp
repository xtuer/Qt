#include "Window.h"
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    // 退出窗口的快捷键
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), &w);
    QObject::connect(shortcut, &QShortcut::activated, [] {
        qApp->quit();
    });

    return app.exec();
}
