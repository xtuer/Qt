#ifndef MAGICWINDOW_H
#define MAGICWINDOW_H

#include <QWidget>
#include <QPixmap>

class MagicWindowPrivate;

namespace Ui {
class MagicWindow;
}

class MagicWindow : public QWidget {
    Q_OBJECT

public:
    explicit MagicWindow(QWidget *centralWidget, bool asDialog = false);
    ~MagicWindow();

    /**
     * @brief 设置标题
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief 设置是否显示最小化，最大化，关闭按钮
     *
     * @param min 为 true 时显示最小化按钮，为 false 时不显示
     * @param max 为 true 时显示最大化按钮，为 false 时不显示
     * @param close 为 true 时显示关闭按钮，为 false 时不显示
     */
    void showButtons(bool min, bool max, bool close);

    /**
     * @brief 设置是否可以修改窗口的大小，默认为可以修改窗口大小
     *
     * @param resizable 为 true 时可以修改窗口的大小，为 false 时不可以
     */
    void setResizable(bool resizable);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    void signalSlot();                   // 处理信号槽
    bool isMouseAtEdge() const;          // 检查鼠标是否在窗口边框上
    bool isMoveMode() const;             // 正在移动窗口
    bool isResizeMode() const;           // 正在修改窗口大小
    void calculateMousePosition() const; // 计算鼠标在窗口的哪一个边框上
    void updateCursor();                 // 根据鼠标的位置更新鼠标的样式
    void reset();                        // 恢复鼠标的标记，鼠标样式等
    QRect centralRect() const;           // 窗口去掉阴影部分的矩形区域，也即真正显示内容的区域

    Ui::MagicWindow *ui;
    MagicWindowPrivate *d;
};


#endif // MAGICWINDOW_H
