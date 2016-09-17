#include "FramelessWindowCentralWidget.h"
#include "ui_FramelessWindowCentralWidget.h"
#include "FramelessWindow.h"

#include <QDebug>
#include <QApplication>
#include <QGridLayout>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

struct FramelessWindowCentralWidgetPrivate {
    FramelessWindow *framelessWindow;             // 顶级无边框窗口
    QPoint mousePressedPosition;                  // 鼠标按下时的全局坐标坐标
    QPoint framelessWindowPositionAsMousePressed; // 鼠标按下时窗口左上角的坐标

    FramelessWindowCentralWidgetPrivate(FramelessWindow *framelessWindow) :  framelessWindow(framelessWindow) {}
};

FramelessWindowCentralWidget::FramelessWindowCentralWidget(FramelessWindow *framelessWindow, QWidget *contentWidget, QWidget *parent)
    : QWidget(parent), ui(new Ui::FramelessWindowCentralWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 启用 QSS

    // 添加阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(Qt::lightGray);
    shadowEffect->setBlurRadius(4); // 阴影的大小
    shadowEffect->setOffset(0, 0);
    setGraphicsEffect(shadowEffect);

    // 初始化数据
    d = new FramelessWindowCentralWidgetPrivate(framelessWindow);

    // 添加 contentWidget
    (qobject_cast<QGridLayout*>(layout()))->addWidget(contentWidget, 1, 0);

    // 最小化
    connect(ui->minimizeButton, &QPushButton::clicked, [this] {
        d->framelessWindow->showMinimized();
    });

    // 最大化
    connect(ui->maximizeButton, &QPushButton::clicked, [this] {
        d->framelessWindow->showMaximized();
    });

    // 关闭窗口
    connect(ui->closeButton, &QPushButton::clicked, [this] {
        d->framelessWindow->close();
    });
}

FramelessWindowCentralWidget::~FramelessWindowCentralWidget() {
    delete ui;
    delete d;
}

// 设置最小化，最大化，关闭按钮是否可见，默认都是可见的
void FramelessWindowCentralWidget::setTitleBarButtonsVisible(bool minimizeButtonVisible,
                                                             bool maximizeButtonVisible,
                                                             bool closeButtonVisible) {
    ui->closeButton->setVisible(closeButtonVisible);
    ui->minimizeButton->setVisible(minimizeButtonVisible);
    ui->maximizeButton->setVisible(maximizeButtonVisible);
}

// 设置窗口的标题
void FramelessWindowCentralWidget::setTitle(const QString &title) {
    ui->titleLabel->setText(title);
}

// 设置是否显示标题栏
void FramelessWindowCentralWidget::setTitleBarVisible(bool visible) {
    ui->titleBarWidget->setVisible(visible);
}

void FramelessWindowCentralWidget::mousePressEvent(QMouseEvent *e) {
    e->accept();
    d->mousePressedPosition = e->globalPos();
    d->framelessWindowPositionAsMousePressed = d->framelessWindow->pos();
}

void FramelessWindowCentralWidget::mouseReleaseEvent(QMouseEvent *e) {
    e->accept();
    d->mousePressedPosition = QPoint();
}

void FramelessWindowCentralWidget::mouseMoveEvent(QMouseEvent *e) {
    e->accept();

    if (!d->mousePressedPosition.isNull()) {
        QPoint delta = e->globalPos() - d->mousePressedPosition;
        QPoint pos = d->framelessWindowPositionAsMousePressed + delta;
        d->framelessWindow->move(pos);
    }
}
