#include "FramelessWindow.h"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建包含主要控件的 Widget
    QPushButton *quitButton = new QPushButton("退出");
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("按住我拖动也可以拖动窗口的哦"));
    layout->addWidget(new QTextEdit());
    layout->addWidget(quitButton);

    QWidget *contentWidget = new QWidget();
    contentWidget->setLayout(layout);

    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet("#contentWidget {background: #EEE; border-bottom-left-radius: 4px;border-bottom-right-radius: 4px;}"
                                 ".QLabel{background: gray;}.QTextEdit{background: white;}");

    // 创建无边框、圆角、有阴影、可拖动的窗口
    FramelessWindow *window = new FramelessWindow(contentWidget);
    window->setTitleBarButtonsVisible(true, false, true);
    window->setTitle("客户就是上帝");
//    window->setTitleBarVisible(false);
    window->resize(300, 400);
//    window->setResizable(false);
    window->show();

    return app.exec();
}
