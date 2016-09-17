#include "FramelessWindow.h"
#include "FramelessWindowCentralWidget.h"

#include <QDebug>
#include <QRect>
#include <QMouseEvent>
#include <QGridLayout>
#include <QRubberBand>
#include <QApplication>

struct FramelessWindowPrivate {
    FramelessWindowPrivate(FramelessWindowCentralWidget *centralWidget)
        : resizable(true), rubberBand(new QRubberBand(QRubberBand::Rectangle)), centralWidget(centralWidget) {
    }

    ~FramelessWindowPrivate() {
        delete rubberBand;
    }

    bool left;   // 为 true 时鼠标在窗口的左边框
    bool right;  // 为 true 时鼠标在窗口的右边框
    bool top;    // 为 true 时鼠标在窗口的上边框
    bool bottom; // 为 true 时鼠标在窗口的下边框

    bool resizable; // 为 true 表示可以调整边框大小，为 false 表示不可以调整边框大小

    QPoint startResizePosition; // 开始调整窗口大小时鼠标被按下的位置
    QRect rubberBandRectAsDrag; // 鼠标按下时 rubberBand 的矩形
    QRubberBand *rubberBand;
    FramelessWindowCentralWidget *centralWidget;
};

FramelessWindow::FramelessWindow(QWidget *contentWidget, QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明

    d = new FramelessWindowPrivate(new FramelessWindowCentralWidget(this, contentWidget));
    d->centralWidget->installEventFilter(this);

    // 添加 centralWidget 到窗口中心
    QGridLayout *lo = new QGridLayout();
    lo->addWidget(d->centralWidget, 0, 0);
    lo->setContentsMargins(4, 4, 4, 4);
    setLayout(lo);
}

FramelessWindow::~FramelessWindow() {
    delete d;
}

// 设置最小化，最大化，关闭按钮是否可见，默认都是可见的
void FramelessWindow::setTitleBarButtonsVisible(bool minimizeButtonVisible,
                                                bool maximizeButtonVisible,
                                                bool closeButtonVisible) {
    d->centralWidget->setTitleBarButtonsVisible(minimizeButtonVisible, maximizeButtonVisible, closeButtonVisible);
}

// 设置窗口的标题
void FramelessWindow::setTitle(const QString &title) {
    d->centralWidget->setTitle(title);
}

// 设置是否显示标题栏
void FramelessWindow::setTitleBarVisible(bool visible) {
    d->centralWidget->setTitleBarVisible(visible);
}

// 设置是否可以调整窗口的大小
void FramelessWindow::setResizable(bool resizable) {
    d->resizable = resizable;
}

void FramelessWindow::mousePressEvent(QMouseEvent *e) {
    if (isMouseAtEdge()) {
        // 在窗口边框上按下鼠标时，记下鼠标的位置，并且显示 rubberBand，进入调整窗口大小的模式
        // 只有鼠标在窗口的边上按下时才记录鼠标按下的位置
        d->startResizePosition = e->globalPos();

        // 鼠标按下时 rubberBand 的大小为 centralWidget 的大小(使用的是全局坐标)
        d->rubberBandRectAsDrag = QRect(d->centralWidget->mapToGlobal(QPoint(0,0)), d->centralWidget->size());
        d->rubberBand->setGeometry(d->rubberBandRectAsDrag);
        d->rubberBand->show();

        // 开始调整窗口大小的时候设置全局的鼠标样式，否则进入 d->centralWidget 后鼠标的样式会改变
        if (d->left || d->right) {
            QApplication::setOverrideCursor(Qt::SizeHorCursor);
        } else if (d->top || d->bottom) {
            QApplication::setOverrideCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)

    if (isResizeMode()) {
        // 调整窗口大小的模式下松开鼠标时修改窗口的大小
        QSize ds = size() - d->rubberBandRectAsDrag.size(); // 开始调整窗口大小时窗口和 rubberBand 的大小差值
        int dw   = ds.width() / 2;
        int dh   = ds.height() / 2;
        setGeometry(QRect(d->rubberBand->pos() - QPoint(dw, dh), d->rubberBand->size() + QSize(dw+dw, dh+dh)));

        // 隐藏 rubberBand，恢复鼠标样式
        reset();
        d->rubberBand->hide();
        QApplication::restoreOverrideCursor();
    }
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *e) {
    Q_UNUSED(e)

    // 不可调整窗口大小时返回
    if (!d->resizable) { return; }

    if (isResizeMode()) {
        // 调整窗口大小的模式下移动鼠标时改变 rubberBand 的大小
        QRect r(d->rubberBandRectAsDrag);
        int x = QCursor::pos().x();
        int y = QCursor::pos().y();

        if (d->left) {
            r.setLeft(x);
        } else if (d->top) {
            r.setTop(y);
        } else if (d->right) {
            r.setRight(x);
        } else if (d->bottom) {
            r.setBottom(y);
        }

        d->rubberBand->setGeometry(r);
    } else {
        // 非调整窗口大小的模式下检查鼠标的位置并更新鼠标的样式
        calculateMousePosition();
        updateCursor();
    }
}

bool FramelessWindow::eventFilter(QObject *watched, QEvent *e) {
    // 当鼠标离开 framelessWindow 进入 centralWidget，并且不是调整窗口大小的模式时恢复鼠标的样式和边框标记等，否则鼠标的样式会显示异常
    if (watched == d->centralWidget && e->type() == QEvent::Enter && !isResizeMode()) {
        reset();
    }

    return QWidget::eventFilter(watched, e);
}

// 计算鼠标在窗口的哪一个边框上
void FramelessWindow::calculateMousePosition() const {
    // 使用 central widget 的范围作为参考来计算鼠标是否在边框上效果更好一些
    QRect rect(d->centralWidget->mapToGlobal(QPoint(0, 0)), d->centralWidget->size());
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();

    // 4 个边的矩形，宽或高为 2 个像素
    QRect leftRect(x-2, y, 2, h);
    QRect rightRect(x+w+1, y, 2, h);
    QRect topRect(x, y-2, w, 2);
    QRect bottomRect(x, y+h+1, w, 2);

    QPoint pos = QCursor::pos();
    d->left    = leftRect.contains(pos);
    d->right   = rightRect.contains(pos);
    d->top     = topRect.contains(pos);
    d->bottom  = bottomRect.contains(pos);
}

// 检查鼠标是否在窗口边框上
bool FramelessWindow::isMouseAtEdge() const {
    return d->left || d->right || d->top || d->bottom;
}

// 检查是否调整窗口大小的模式
bool FramelessWindow::isResizeMode() const {
    return !d->startResizePosition.isNull();
}

// 根据鼠标的位置更新鼠标的样式
void FramelessWindow::updateCursor() {
    if (d->left || d->right) {
        setCursor(Qt::SizeHorCursor);
    } else if (d->top || d->bottom) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

// 恢复鼠标的标记，鼠标样式等
void FramelessWindow::reset() {
    d->left   = false;
    d->right  = false;
    d->top    = false;
    d->bottom = false;
    setCursor(Qt::ArrowCursor);
    d->startResizePosition = QPoint();
}
