#include "TopWindow.h"
#include "ui_TopWindow.h"
#include "util/NinePatchPainter.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QSizeGrip>

TopWindow::TopWindow(QWidget *centralWidget) : ui(new Ui::TopWindow) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    ninePatchPainter = new NinePatchPainter(QPixmap(":/image/top-window/shadow.png"), 23, 12, 23, 33);

    QGridLayout *l = qobject_cast<QGridLayout *>(layout());

    // 替换中心的 centralWidget
    delete l->replaceWidget(ui->centralWidget, centralWidget);
    delete ui->centralWidget;

    // 缩放窗口的 QSizeGrip
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setStyleSheet("width: 15px; height: 15px; background: red;");
    l->addWidget(sizeGrip, 1, 0, Qt::AlignRight | Qt::AlignBottom);

    // 点击按钮 X 退出程序
    connect(ui->closeButton, &QPushButton::clicked, [this] {
        close();
    });
}

TopWindow::~TopWindow() {
    delete ui;
    delete ninePatchPainter;
}

void TopWindow::setTitle(const QString &title) {
    ui->titleLabel->setText(title);
}

void TopWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    ninePatchPainter->paint(&painter, rect());
}

// 鼠标按下时记录此时鼠标的全局坐标和窗口左上角的坐标
void TopWindow::mousePressEvent(QMouseEvent *event) {
    mousePressedPosition = event->globalPos();
    windowPositionBeforeMoving = frameGeometry().topLeft();
}

// 鼠标放开时设置 mousePressedPosition 为空坐标
void TopWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    mousePressedPosition = QPoint();
}

// 鼠标移动时如果 mousePressedPosition 不为空，则说明需要移动窗口
// 鼠标移动的位移差，就是窗口移动的位移差
void TopWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!mousePressedPosition.isNull()) {
        QPoint delta = event->globalPos() - mousePressedPosition;
        QPoint newPosition = windowPositionBeforeMoving + delta;
        move(newPosition);
    }
}
