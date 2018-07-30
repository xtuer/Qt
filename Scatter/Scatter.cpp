#include "Scatter.h"
#include "ScatterMap.h"

#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QToolTip>
#include <QCursor>

Scatter::Scatter(double xRatio, double yRatio, QWidget *parent) : QWidget(parent), xRatio(xRatio), yRatio(yRatio) {
    setAttribute(Qt::WA_StyledBackground);
    createContextMenu(); // 创建右键菜单
}

// 设置坐标
void Scatter::setPosition(double xRatio, double yRatio) {
    this->xRatio = xRatio;
    this->yRatio = yRatio;
    updatePosition();
}

// 更新坐标
void Scatter::updatePosition() {
    int x = static_cast<int>(parentWidget()->width() * xRatio);  // - width()/2;
    int y = static_cast<int>(parentWidget()->height() * yRatio); // - height()/2;
    move(x, y);

    emit positionChanged(QPoint(x, y));
}

// 获取 scatter 在父 widget 中的横坐标，使用比例
double Scatter::getXRatio() const {
    return xRatio;
}

// 获取 scatter 在父 widget 中的纵坐标，使用比例
double Scatter::getYRatio() const {
    return yRatio;
}

// 获取 scatter 的坐标，使用绝对坐标
QPoint Scatter::getPosition() const {
    ScatterMap *scatterMap = qobject_cast<ScatterMap *>(parentWidget());

    if (nullptr != scatterMap) {
        int x = static_cast<int>(scatterMap->getScatterMapWidth() * xRatio);
        int y = static_cast<int>(scatterMap->getScatterMapHeight() * yRatio);
        return QPoint(x, y);
    }

    return QPoint();
}

// 鼠标按下时记录此时鼠标的全局坐标和 widget 左上角的坐标
void Scatter::mousePressEvent(QMouseEvent *event) {
    mousePressedPosition = event->globalPos();
    topLeftPositionBeforeMoving = geometry().topLeft();
}

// 鼠标放开时设置 mousePressedPosition 为空坐标
void Scatter::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    mousePressedPosition = QPoint();
}

// 鼠标移动时如果 mousePressedPosition 不为空，则说明需要移动 widget
// 鼠标移动的位移差，就是 widget移动的位移差
void Scatter::mouseMoveEvent(QMouseEvent *event) {
    if (!mousePressedPosition.isNull()) {
        QPoint delta = event->globalPos() - mousePressedPosition;
        QPoint newPosition = topLeftPositionBeforeMoving + delta;

        // 但位置变化后，更新在父 widget 中的 xRatio 和 yRatio
        int x = newPosition.x();
        int y = newPosition.y();
        int tw = width();
        int th = height();
        int pw = parentWidget()->width();
        int ph = parentWidget()->height();

        // 控制拖动时不超出范围
        x = qMax(0, x);
        x = qMin(x, pw-tw);
        y = qMax(0, y);
        y = qMin(y, ph-th);

        this->xRatio = static_cast<double>(x) / pw;
        this->yRatio = static_cast<double>(y) / ph;
        updatePosition();
    }
}

// 鼠标移动进入时显示坐标的 tool tip
void Scatter::enterEvent(QEvent *event) {
    Q_UNUSED(event)

    QPoint pos = getPosition();
    QString coordinator = QString("[X: %1, Y: %2]").arg(pos.x()).arg(pos.y());
    QToolTip::showText(QCursor::pos(), coordinator);
}

// 创建右键菜单
void Scatter::createContextMenu() {
    setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *deleteAction = new QAction("删除", this);

    connect(deleteAction, &QAction::triggered, [this] {
        emit aboutToRemove(this);
    });

    connect(this, &QWidget::customContextMenuRequested, [=](const QPoint &pos) {
        QMenu menu;
        menu.addAction(deleteAction);
        menu.exec(mapToGlobal(pos));
    });
}
