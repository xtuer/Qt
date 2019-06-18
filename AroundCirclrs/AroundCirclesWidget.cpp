#include "AroundCirclesWidget.h"

#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QRectF>
#include <QMatrix>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

// 圆
struct Circle {
    int     id;         // ID
    QString name;       // 名字
    QColor  color;      // 前景色
    QColor  bgcolor;    // 背景色
    QMatrix matrix;     // 变换矩阵，圆的最终位置通过 matrix 变换后得到
    QRectF  rect;       // 圆的矩形访问，坐标为原点
    QPainterPath shape; // 圆的最终区域，用户碰撞检测
    bool    hover;      // 是否鼠标放在上面
};

/*-----------------------------------------------------------------------------|
 |                         AroundCirclesWidgetPrivate                          |
 |----------------------------------------------------------------------------*/
class AroundCirclesWidgetPrivate {
    friend class AroundCirclesWidget;

    AroundCirclesWidgetPrivate();
    ~AroundCirclesWidgetPrivate();

    /**
     * 计算紧紧围绕半径为 r 的大圆的 n 个小圆的半径 (大圆和小圆相切，小圆之间相切)
     *
     * @param r 大圆的半径
     * @param n 小圆的个数
     * @return 返回小圆的半径
     */
    double aroundCircleRadius(double r, int n) const;

    /**
     * 创建绕大圆的小圆
     */
    void buildAroundCircles();

    /**
     * 查找坐标 pos 处的小圆
     *
     * @param pos 坐标
     * @return 返回找到的小圆，找不到返回 nullptr
     */
    Circle* findCircle(QPoint pos) const;

    // 设置 id 为传入的 id 的小圆的背景色
    void setCircleColor(int id, const QColor bgcolor);

    // 设置 circle 的小圆的背景色
    void setCircleColor(Circle *circle, const QColor bgcolor);

    // 清除名字为 name 的小圆的背景色，并且把它的名字设置为空
    void clearCircles(const QString &name);

    /**
     * 清除小圆的 hover 状态
     */
    void clearCircleHover();

    /**
     * 在 painter 中绘制小圆，matrix 为 painter 的变换矩阵
     */
    void drawCircles(QPainter *painter, QMatrix matrix);

    int     n = 16;        // 绕大圆的第一圈小圆的个数
    double cr = 100;       // 中心大圆的半径
    QList<Circle*> circles; // 小圆
};

AroundCirclesWidgetPrivate::AroundCirclesWidgetPrivate() {
    buildAroundCircles();
}

AroundCirclesWidgetPrivate::~AroundCirclesWidgetPrivate() {
    qDeleteAll(circles);
}

// 创建绕大圆的小圆
void AroundCirclesWidgetPrivate::buildAroundCircles() {
    // 1. 计算小圆的半径
    // 2. 生成第一圈绕大圆的小圆
    // 3. 生成第二圈绕大圆的小圆
    // 4. 生成小圆的 path，用于碰撞检测

    // [1] 计算小圆的半径
    double sr = aroundCircleRadius(cr, n);
    int    sn = 1; // 小圆的序号，作为小圆的 id

    // [2] 生成第一圈绕大圆的小圆
    double br1 = cr+sr;
    int    n1  = n; // 第一圈小圆的个数

    for (int i = 0; i < n1; ++i) {
        QMatrix matrix;
        matrix.rotate(-360.0/n1*i);
        matrix.translate(0, br1+15);

        Circle *circle  = new Circle();
        circle->id      = sn++;
        circle->color   = Qt::black;
        circle->bgcolor = Qt::transparent;
        circle->rect    = QRectF(-sr, -sr, 2*sr, 2*sr);
        circle->matrix  = matrix;
        circle->hover   = false;

        circles << circle;
    }

    // [3] 生成第二圈绕大圆的小圆
    double br2 = cr+sr*3;
    int    n2  = int(M_PI*br2 / sr); // 第二圈小圆的个数

    for (int i = 0; i < n2; ++i) {
        QMatrix matrix;
        matrix.rotate(-360.0/n2*i);
        matrix.translate(0, br2+20);

        Circle *circle = new Circle();
        circle->id      = sn++;
        circle->color   = Qt::black;
        circle->bgcolor = Qt::transparent;
        circle->rect    = QRectF(-sr, -sr, 2*sr, 2*sr);
        circle->matrix  = matrix;
        circle->hover   = false;

        circles << circle;
    }

    // [4] 生成小圆的 path，用于碰撞检测
    for (Circle *circle : circles) {
        QPainterPath shape;
        shape.addEllipse(circle->rect);

        circle->shape = shape * circle->matrix;
    }
}

// 查找坐标 pos 处的小圆
Circle *AroundCirclesWidgetPrivate::findCircle(QPoint pos) const {
    Circle *found = nullptr;

    for (Circle *circle : circles) {
        if (circle->shape.contains(pos)) {
            found = circle;
            break;
        }
    }

    return found;
}

// 清除小圆的 hover 状态
void AroundCirclesWidgetPrivate::clearCircleHover() {
    for (Circle *circle : circles) {
        circle->hover = false;
    }
}

// 计算紧紧围绕半径为 r 的大圆的 n 个小圆的半径 (大圆和小圆相切，小圆之间相切)
double AroundCirclesWidgetPrivate::aroundCircleRadius(double r, int n) const {
    // 圆的内切正 n 边型的边长为 2*r*sin(π/n)
    // 小圆的半径为 x, 计算半径为 r+x 的圆的 2n 个内切正多边形的边长 b, b 等于 x
    double t = qSin(M_PI/2/n);
    return (2*r*t) / (1-2*t);
}

// 设置 id 为传入的 id 的小圆的背景色
void AroundCirclesWidgetPrivate::setCircleColor(int id, const QColor bgcolor) {
    for (Circle *circle : circles) {
        if (circle->id == id) {
            circle->bgcolor = bgcolor.isValid() ? bgcolor: Qt::transparent;
            break;
        }
    }
}

// 设置 circle 的小圆的背景色
void AroundCirclesWidgetPrivate::setCircleColor(Circle *circle, const QColor bgcolor) {
    circle->bgcolor = bgcolor.isValid() ? bgcolor: Qt::transparent;
}

// 清除名字为 name 的小圆的背景色，并且把它的名字设置为空
void AroundCirclesWidgetPrivate::clearCircles(const QString &name) {
    for (Circle *circle : circles) {
        if (circle->name == name) {
            circle->bgcolor = Qt::transparent;
            circle->name    = "";
        }
    }
}

// 在 painter 中绘制小圆，matrix 为 painter 的变换矩阵
void AroundCirclesWidgetPrivate::drawCircles(QPainter *painter, QMatrix matrix) {
    for (Circle *circle : circles) {
        painter->save();

        // 圆上应用 painter 的 matrix
        // 例如 circle 的坐标是使用理论上的原点 (0, 0)，这个原点不是 painter 的坐标原点，
        // 所以需要先移动 painter 的原点，然后再应用 circle 的 matrix
        painter->setMatrix(circle->matrix * matrix);

        // hover 状态
        if (circle->hover) {
            painter->setPen(Qt::red);
        } else {
            painter->setPen(circle->color);
        }

        painter->setBrush(circle->bgcolor);
        painter->drawEllipse(circle->rect);
        painter->drawText(circle->rect, Qt::AlignCenter, QString::number(circle->id));

        painter->restore();
    }
}

/*-----------------------------------------------------------------------------|
 |                             AroundCirclesWidget                             |
 |----------------------------------------------------------------------------*/
AroundCirclesWidget::AroundCirclesWidget(QWidget *parent) : QWidget(parent) {
    d = new AroundCirclesWidgetPrivate();

    setMouseTracking(true);
    setAcceptDrops(true);
}

AroundCirclesWidget::~AroundCirclesWidget() {
    delete d;
}

// 设置 id 为传入的 id 的小圆的背景色
void AroundCirclesWidget::setCircleColor(int id, const QColor bgcolor) {
    d->setCircleColor(id, bgcolor);
}

void AroundCirclesWidget::paintEvent(QPaintEvent *) {
    // 1. 移动坐标原点到 widget 的中心
    // 2. 绘制中心的大圆
    // 3. 绘制大圆上的序号: 逆时针，1 到 n
    // 4. 绘制小圆

    // [1] 移动坐标原点到 widget 的中心
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() / 2, height() / 2);

    // [2] 绘制中心的大圆
    double br = d->cr+8; // 中心大圆的半径: +8 调整效果
    painter.drawEllipse(QRectF(-br, -br, 2*br, 2*br));

    // [3] 绘制大圆上的序号: 逆时针，1 到 n
    for (int i = 0; i < d->n; ++i) {
        painter.save();
        painter.rotate(-360.0/d->n*i);
        painter.translate(0, d->cr-5);
        painter.drawText(QRectF(-8, -8, 16, 16), Qt::AlignCenter, QString::number(i+1));
        painter.restore();
    }

    // [4] 绘制小圆
    QMatrix matrix;
    matrix.translate(width()/2, height()/2); // 原点移动到窗口中心
    d->drawCircles(&painter, matrix);
}

// 拖拽进入
void AroundCirclesWidget::dragEnterEvent(QDragEnterEvent *event) {
    // 当拖放时鼠标进入时, 接受拖放的动作
    QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
    dee->acceptProposedAction();
}

// 拖拽移动时，找到鼠标指向的圆，设置它的 hover 为 true
void AroundCirclesWidget::dragMoveEvent(QDragMoveEvent *event) {
    QPoint pos = event->pos() - QPoint(width()/2, height()/2);

    d->clearCircleHover();

    Circle *circle = d->findCircle(pos);
    if (nullptr != circle) {
        circle->hover = true;
    }

    update();
}

// 拖放操作
void AroundCirclesWidget::dropEvent(QDropEvent *event) {
    event->accept();

    QPoint pos = event->pos() - QPoint(width()/2, height()/2);
    Circle *circle = d->findCircle(pos);

    if (nullptr == circle) {
        d->clearCircleHover();
        update();
        return;
    }

    const QMimeData *mimeData = event->mimeData(); // 获得 drop 的数据

    if (mimeData->hasFormat("DnD-DEVICE-CIRCLE")) {
        // 数据格式: name,color
        QStringList data = QString::fromUtf8(mimeData->data("DnD-DEVICE-CIRCLE")).split(",");
        QString name  = data.value(0);
        QString color = data.value(1);

        d->clearCircles(name);
        d->setCircleColor(circle, QColor(color));
        circle->name = name;
    }

    d->clearCircleHover();
    update();
}




