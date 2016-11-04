#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>

struct FramelessWindowPrivate;

class FramelessWindow : public QWidget {
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *centralWidget, QWidget *parent = 0);
    ~FramelessWindow();

    /**
     * @brief 设置最小化，最大化，关闭按钮是否可见，默认都是可见的
     *
     * @param minimizeButtonVisible 为 true 则最小化按钮可见，否则不可见
     * @param maximizeButtonVisible 为 true 则最大化按钮可见，否则不可见
     * @param closeButtonVisible    为 true 则关闭按钮可见，否则不可见
     */
    void setTitleBarButtonsVisible(bool minimizeButtonVisible = true,
                                   bool maximizeButtonVisible = true,
                                   bool closeButtonVisible = true);

    /**
     * @brief 设置窗口的标题
     * @param title 标题
     */
    void setTitle(const QString &title);

    /**
     * @brief 设置是否显示标题栏
     * @param visible 为 true 则标题栏可见，否则不可见
     */
    void setTitleBarVisible(bool visible);

    /**
     * @brief 设置是否可以调整窗口的大小
     * @param resizable
     */
    void setResizable(bool resizable);

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

private:
    void calculateMousePosition() const; // 计算鼠标在窗口的哪一个边框上
    bool isMouseAtEdge() const;          // 检查鼠标是否在窗口边框上
    bool isResizeMode() const;           // 检查是否调整窗口大小的模式
    void updateCursor();                 // 根据鼠标的位置更新鼠标的样式
    void reset();                        // 恢复鼠标的标记，鼠标样式等

    FramelessWindowPrivate *d;
};

#endif // FRAMELESSWINDOW_H
