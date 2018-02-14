#include "DemoWidget.h"
#include "ui_DemoWidget.h"
#include "TopWindow.h"

#include <QDebug>

DemoWidget::DemoWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DemoWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("DemoWidget {background: white;} ");

    // 点击按钮关闭窗口
    connect(ui->closeButton, &QPushButton::clicked, [this] {
        // 找到最顶层的 widget 并关闭它
        QWidget *p = this;
        while (NULL != p->parentWidget()) {
            p = p->parentWidget();
        }
        p->close();
    });

    // 简单消息对话框
    connect(ui->simpleMessageButton, &QPushButton::clicked, [this] {
        TopWindow::message("花果山果汁科技信息技术有限公司\n法人是齐天大圣"); // 换行用 \n
    });

    // 复杂消息对话框
    connect(ui->complexMessageButton, &QPushButton::clicked, [this] {
        TopWindow::message("<b>公司</b>: 花果山果汁科技信息技术有限公司<br>"
                           "<b>法人</b>: 齐天大圣<br>"
                           "<b>版本</b>: Release 1.1.3<br>"
                           "<center><img src=\":/image/top-window/logo.png\" width=64 height=64></center>", 350, 140);
    });

    // 阻塞模态对话框
    connect(ui->modalDialogButton, &QPushButton::clicked, [] {
        DemoWidget *c = new DemoWidget();
        TopWindow window(c);
        window.showModal();

        // 直到关闭 window，程序才继续往下运行
        qDebug() << c->getStatus();
    });
}

DemoWidget::~DemoWidget() {
    delete ui;

    qDebug() << "~DemoWidget()";
}
