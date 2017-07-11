#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H
#include <QLabel>

class DraggableLabel : public QLabel {
public:
    DraggableLabel(const QString &text, QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QRect resizeHandle() const;

    QPoint pressedPos;
    QPoint posAsPressed;
    QSize  sizeAsPressed;
    bool resizeMode;
};

#endif // DRAGGABLELABEL_H
