#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H
#include <QLabel>

/**
 * 可以拖拽移动位置和修改大小的 label。
 * 当鼠标按住右下角的蓝色小方块拖动时修改 label 的大小，按住其他地方拖动时移动 label。
 */
class DraggableLabel : public QLabel {
public:
    DraggableLabel(const QString &text, QWidget *parent = Q_NULLPTR);

    // 操作模式
    enum ActionMode {
        NONE,  // 无，也可以认为是不识别的模式
        MOVE,  // 移动模式
        RESIZE // 修改大小模式
    };

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QRect resizeHandle() const; // 拖动修改 label 大小的 handle，位于 label 右下角

    QPoint pressedPos;    // 鼠标按下的位置
    QPoint posAsPressed;  // 鼠标按下时 label 左上角的位置，
    QSize  sizeAsPressed; // 鼠标按下时 label 的大小
    ActionMode mode; // RESIZE 为缩放大小模式，MOVE 为移动模式
};

#endif // DRAGGABLELABEL_H
