#include "DraggableLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

DraggableLabel::DraggableLabel(const QString &text, QWidget *parent) : QLabel(text, parent), mode(ActionMode::NONE) {
    setAttribute(Qt::WA_StyledBackground, true);
    setProperty("class", "DraggableLabel");
    setStyleSheet("border: 1px solid darkblue; background: white;");
}

void DraggableLabel::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // 居中绘制 label 的文本
    painter.drawText(rect(), Qt::AlignCenter, text());

    // 绘制右下角用于移动 label 的小方块
    painter.setBrush(Qt::darkBlue);
    painter.drawRect(resizeHandle());
}

void DraggableLabel::mousePressEvent(QMouseEvent *event) {
    // 鼠标按下时记录鼠标按下的位置，label 在 parent 上的位置，label 的大小
    // 如果按下的位置在右下角的小方块上则为修改大小模式，否则为移动模式
    event->accept();
    pressedPos = event->globalPos();
    posAsPressed = pos();
    sizeAsPressed = size();
    mode = resizeHandle().contains(event->pos()) ? ActionMode::RESIZE : ActionMode::MOVE;
}

void DraggableLabel::mouseReleaseEvent(QMouseEvent *event) {
    event->accept();
    mode = ActionMode::NONE; // 松开鼠标时操作模式为 NONE
}

void DraggableLabel::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    // 鼠标移动时相对于按下时偏移的位置，用全局坐标计算防止累积误差
    QPoint d = event->globalPos() - pressedPos;

    if (ActionMode::RESIZE == mode) { // 修改大小
        this->resize(sizeAsPressed + QSize(d.x(), d.y()));
    } else if (ActionMode::MOVE == mode) { // 移动
        this->move(posAsPressed + d);
    }
}

// 拖动修改 label 大小的 handle，位于 label 右下角，大小是 8x8 的矩形
QRect DraggableLabel::resizeHandle() const {
    int s = 6;
    return QRect(width() - s, height() - s, s, s);
}
