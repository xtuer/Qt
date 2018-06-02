#include "ScatterMap.h"
#include "Scatter.h"

#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QDebug>

ScatterMap::ScatterMap(QWidget *parent) : QWidget(parent) {
    createContextMenu();
}

// 获取布点地图的宽
int ScatterMap::getScatterMapWidth() const {
    return scatterMapWidth;
}

// 获取布点地图的高
int ScatterMap::getScatterMapHeight() const {
    return scatterMapHeight;
}

// 设置布点地图的宽
void ScatterMap::setScatterMapWidth(int width) {
    scatterMapWidth = width;
}

// 设置布点地图的高
void ScatterMap::setScatterMapHeight(int height) {
    scatterMapHeight = height;
}

// 获取所有 scatter 的坐标
QList<QPoint> ScatterMap::getScatterPositions() const {
    QList<QPoint> positions;

    for (Scatter *scatter : scatters) {
        positions.append(scatter->getPosition());
    }

    return positions;
}

void ScatterMap::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    int w = width();
    int h = height();
    double deltaX = w / 10.0;
    double deltaY = h / 10.0;

    painter.setPen(QPen(Qt::gray, 0));

    // 画横线
    for (double y = deltaY; y < h-1; y += deltaY) {
        painter.drawLine(QPointF(0, y), QPointF(w, y));
    }

    // 画纵线
    for (double x = deltaX; x < w-1; x += deltaX) {
        painter.drawLine(QPointF(x, 0), QPointF(x, h));
    }

    painter.drawRect(0, 0, w-1, h-1);
}

void ScatterMap::resizeEvent(QResizeEvent *event) {
    for (Scatter *scatter : scatters) {
        scatter->updatePosition();
    }

    QWidget::resizeEvent(event);
}

void ScatterMap::removeScatter(Scatter *scatter) {
    scatters.removeOne(scatter);
    scatter->deleteLater();
}

// 创建右键菜单
void ScatterMap::createContextMenu() {
    setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *addAction = new QAction("添加", this);

    connect(addAction, &QAction::triggered, [this] {
        addScatter(0, 0);
    });

    connect(this, &QWidget::customContextMenuRequested, [=](const QPoint &pos) {
        QMenu menu;
        menu.addAction(addAction);
        menu.exec(mapToGlobal(pos));
    });
}

// 创建 Scatter
void ScatterMap::addScatter(double xRatio, double yRatio) {
    Scatter *scatter = new Scatter(xRatio, yRatio, this);
    scatter->show();
    scatters.append(scatter);

    connect(scatter, &Scatter::aboutToRemove, this, &ScatterMap::removeScatter);
    connect(scatter, &Scatter::positionChanged, [=] {
        emit scatterPositionChanged(scatter, scatter->getPosition());
    });
}
