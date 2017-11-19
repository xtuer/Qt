#ifndef MOUSESELECTIONWIDGET_H
#define MOUSESELECTIONWIDGET_H

#include <QWidget>

class MouseSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit MouseSelectionWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QRect rect;       // 鼠标拖拽出的矩形
    bool  modifyMode; // 为 true 表示编辑矩形模式，为 false 表示不再修改矩形模式
};

#endif // MOUSESELECTIONWIDGET_H
