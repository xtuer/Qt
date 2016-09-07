#include "FramelessWidget.h"
#include <QMouseEvent>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>

FramelessWidget::FramelessWidget(QWidget *centralWidget, QWidget *parent) : QWidget(parent) {
    // 添加阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(centralWidget);
    shadowEffect->setColor(Qt::lightGray);
    shadowEffect->setBlurRadius(4); // 阴影的大小
    shadowEffect->setOffset(0, 0);
    centralWidget->setGraphicsEffect(shadowEffect);
    centralWidget->setAttribute(Qt::WA_StyledBackground, true); // 启用 QSS
    centralWidget->setObjectName("centralWidget");
    centralWidget->setStyleSheet(centralWidget->styleSheet() + "#centralWidget {border-radius: 4px}"); // 设置圆角

    QGridLayout *lo = new QGridLayout();
    lo->addWidget(centralWidget, 0, 0);
    lo->setContentsMargins(4, 4, 4, 4);
    setLayout(lo);

    setWindowFlags(Qt::FramelessWindowHint);     // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground);  // 背景透明
}

void FramelessWidget::mousePressEvent(QMouseEvent *e) {
    // 记录鼠标按下时全局的位置和窗口左上角的位置
    pressedMousePosition = e->globalPos();
    topLeftPositionBeforeDrag = frameGeometry().topLeft();
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)
    // 鼠标放开始设置鼠标按下的位置为 null，表示鼠标没有被按下
    pressedMousePosition = QPoint();
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *e) {
    // 鼠标按下并且移动时，移动窗口
    if (!pressedMousePosition.isNull()) {
        QPoint delta = e->globalPos() - pressedMousePosition;
        move(topLeftPositionBeforeDrag + delta); // 移动窗口
    }
}
