#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class FramelessWindow;
struct FramelessWindowCentralWidgetPrivate;

namespace Ui {
class FramelessWindowCentralWidget;
}

/**
 * @brief 类 FramelessWindowCentralWidget 负责窗口的移动和阴影
 */
class FramelessWindowCentralWidget : public QWidget {
    Q_OBJECT

public:
    explicit FramelessWindowCentralWidget(FramelessWindow *framelessWindow, QWidget *contentWidget, QWidget *parent = 0);
    ~FramelessWindowCentralWidget();

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

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    Ui::FramelessWindowCentralWidget *ui;
    FramelessWindowCentralWidgetPrivate *d;
};

#endif // CENTRALWIDGET_H
