#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QWidget>

class TopWindowPrivate;

namespace Ui {
class TopWindow;
}

class TopWindow : public QWidget {
    Q_OBJECT

public:
    explicit TopWindow(QWidget *centralWidget,
                       const QMargins &windowPaddings     = QMargins(16, 10, 16, 16),
                       const QMargins &borderImageBorders = QMargins(23, 13, 23, 33),
                       const QString  &borderImagePath    = QString(":/image/shadow.png"),
                       bool  borderImageHorizontalStretch = true,
                       bool  borderImageVerticalStretch   = true);
    ~TopWindow();

    /**
      * @brief 设置标题
      *
      * @param title
      */
     void setTitle(const QString &title);

     /**
      * @brief  设置是否显示标题栏，有时候需要自定义复杂的工具栏，并且在工具栏上有关闭等按钮
      *
      * @param visible 为 true 时显示标题栏，为 false 时不显示
      */
     void setTitleBarVisible(bool visible = true);

     /**
      * @brief 设置是否显示最小化，最大化，关闭按钮
      *
      * @param min   为 true 时显示最小化按钮，为 false 时不显示
      * @param max   为 true 时显示最大化按钮，为 false 时不显示
      * @param close 为 true 时显示关闭按钮，为 false 时不显示
      */
     void setTitleBarButtonsVisible(bool min, bool max, bool close);

    /**
     * @brief 设置是否可以修改窗口的大小，默认为可以修改窗口大小
     *
     * @param resizable 为 true 时可以修改窗口的大小，为 false 时不可以
     */
    void setResizable(bool resizable);

     /**
      * @brief 最大化窗口
      */
     void showMaximized();

     /**
      * @brief 从最大化恢复普通窗口大小
      */
     void showNormal();

     static TopWindow* message(const QString &msg, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void handleEvents(); // 信号槽事件处理

    Ui::TopWindow *ui;
    TopWindowPrivate *d;
};

#endif // TOPWINDOW_H
