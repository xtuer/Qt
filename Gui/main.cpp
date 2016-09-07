#include "FramelessWidget.h"

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

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    centralWidget->setStyleSheet("#centralWidget{background: lightgray; border: 1px solid lightgray;}"
                                 ".QLabel{background: gray;}.QTextEdit{background: white;}");

    QObject::connect(quitButton, &QPushButton::clicked, [&app] {
        app.quit();
    });

    // 创建无边框、圆角、有阴影、可拖动的窗口
    FramelessWidget *window = new FramelessWidget(centralWidget);
    window->resize(300, 400);
    window->show();

    return app.exec();
}
