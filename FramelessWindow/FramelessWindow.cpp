#include "FramelessWindow.h"
#include "FramelessWindowCentralWidget.h"

#include <QDebug>
#include <QRect>
#include <QMouseEvent>
#include <QGridLayout>
#include <QRubberBand>
#include <QApplication>

struct FramelessWindowPrivate {
    FramelessWindowPrivate(FramelessWindowCentralWidget *centralWidget) : centralWidget(centralWidget), rubberBand(new QRubberBand(QRubberBand::Rectangle)) {}

    ~FramelessWindowPrivate() {
        delete rubberBand;
    }

    bool left;   // 为 true 时鼠标在窗口的左边框
    bool right;  // 为 true 时鼠标在窗口的右边框
    bool top;    // 为 true 时鼠标在窗口的上边框
    bool bottom; // 为 true 时鼠标在窗口的下边框

    QPoint mousePressedPosition; // 鼠标按下时的坐标
    QRubberBand *rubberBand;
    FramelessWindowCentralWidget *centralWidget;
};

FramelessWindow::FramelessWindow(QWidget *contentWidget, QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);     // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground);  // 背景透明

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
void FramelessWindow::setButtonVisibles(bool minimizeButtonVisible, bool maximizeButtonVisible, bool closeButtonVisible) {
    d->centralWidget->setButtonVisibles(minimizeButtonVisible, maximizeButtonVisible, closeButtonVisible);
}

// 设置窗口的标题
void FramelessWindow::setWindowTitle(const QString &title) {
    d->centralWidget->setWindowTitle(title);
}

// 设置是否显示标题栏
void FramelessWindow::setTitleBarVisible(bool visible) {
    d->centralWidget->setTitleBarVisible(visible);
}

void FramelessWindow::mousePressEvent(QMouseEvent *e) {
    if (isMouseAtEdge()) {
        // 在窗口边框上按下鼠标时，记下鼠标的位置，并且显示 rubberBand
        d->mousePressedPosition = e->globalPos();
        d->rubberBand->setGeometry(getRubberBandRectAsDrag());
        d->rubberBand->show();

        // 设置全局的鼠标样式
        if (d->left || d->right) {
            QApplication::setOverrideCursor(Qt::SizeHorCursor);
        } else if (d->top || d->bottom) {
            QApplication::setOverrideCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)

    if (isMousePressed()) {
        // 松开鼠标时修改窗口的大小
        QSize ds = this->size() - d->centralWidget->size();
        int dw   = ds.width() / 2;
        int dh   = ds.height() / 2;

        QPoint pos(d->rubberBand->frameGeometry().topLeft() - QPoint(dw, dh));
        QSize  size(d->rubberBand->width() + dw + dw, d->rubberBand->height() + dh + dh);
        move(pos);
        resize(size);

        // 隐藏 rubberBand，恢复鼠标样式
        reset();
        d->rubberBand->hide();
        QApplication::restoreOverrideCursor();
    }
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *e) {
    if (!isMousePressed()) {
        // 鼠标没有被按下时检查鼠标的位置
        calculateMousePosition();
    } else {
        // 鼠标按下拖放时改变 rubberBand 的大小
        QPoint delta = e->globalPos() - d->mousePressedPosition;
        QRect r = getRubberBandRectAsDrag();

        int dx = delta.x();
        int dy = delta.y();

        int x = r.x();
        int y = r.y();
        int w = r.width();
        int h = r.height();

        if (d->left) {
            x = r.x() + dx;      // 向左时 dx 为负数
            w = r.width() - dx;
        } else if (d->top) {
            y = r.y() + dy;      // 向上时 dy 为负数
            h = r.height() - dy;
        } else if (d->right) {
            w = r.width() + dx;  // 向右时 dx 为正数
        } else if (d->bottom) {
            h = r.height() + dy; // 向下时 dx 为正数
        }

        d->rubberBand->setGeometry(x, y, w, h);
    }

    updateCursor(); // 更行鼠标的样式
}

bool FramelessWindow::eventFilter(QObject *watched, QEvent *e) {
    // 当鼠标离开 framelessWindow 进入 centralWidget，并且鼠标没有按下时恢复鼠标的样式和边框标记
    if (watched == d->centralWidget && e->type() == QEvent::Enter && !isMousePressed()) {
        reset();
    }

    return QWidget::eventFilter(watched, e);
}

// 取得鼠标按下时 rubberBand 的大小
QRect FramelessWindow::getRubberBandRectAsDrag() const {
    // 鼠标按下时 rubberBand 的大小为 centralWidget 的大小
    return QRect(d->centralWidget->mapToGlobal(QPoint(0,0)), d->centralWidget->size());
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

// 检查鼠标是否被按下
bool FramelessWindow::isMousePressed() const {
    return !d->mousePressedPosition.isNull();
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
    d->mousePressedPosition = QPoint();
}
