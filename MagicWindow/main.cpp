#include "MagicWindow.h"
#include <QApplication>
#include <QPushButton>
#include <QLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget *centralWidget = new QWidget();
    centralWidget->setStyleSheet(".QWidget{ background: gray;}");
    QPushButton *button = new QPushButton("模态对话框", centralWidget);
    button->move(100, 50);

    // [1] 普通窗口，显示最大最小和关闭按钮，可调整窗口大小
    MagicWindow window(centralWidget);
    window.setTitle("普通窗口");
    window.resize(500, 300);
    window.show();

    // 点击按钮弹出模态对话框，在任务栏不显示图标
    QObject::connect(button, &QPushButton::clicked, [=] {
        QWidget *centralWidget2 = new QWidget();
        centralWidget2->setStyleSheet("background: #AAA;");

        // [2] 对话框，不显示最大最小按钮，不可调整窗口大小
        MagicWindow *dialog = new MagicWindow(centralWidget2);
        dialog->setTitle("模态对话框");
        dialog->setResizable(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->showModal();
    });

    return a.exec();
}
