#ifndef SCATTERMAP_H
#define SCATTERMAP_H

#include <QWidget>
#include <QList>

class Scatter;

class ScatterMap : public QWidget {
    Q_OBJECT
public:
    explicit ScatterMap(QWidget *parent = nullptr);

    void addScatter(double xRatio, double yRatio); // 创建 Scatter
    int getScatterMapWidth() const;       // 获取布点地图的宽
    int getScatterMapHeight() const;      // 获取布点地图的高
    void setScatterMapWidth(int width);   // 设置布点地图的宽
    void setScatterMapHeight(int height); // 设置布点地图的高

    QList<QPoint> getScatterPositions() const; // 获取所有 scatter 的坐标
    QList<QPoint> getScatterPositionsInParentWidget() const; // 获取所有 scatter 在 parentWidget 上的坐标

signals:
    void scatterPositionChanged(Scatter *scatter, QPoint pos);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void removeScatter(Scatter *scatter);

private:
    void createContextMenu(); // 创建右键菜单

    QList<Scatter *> scatters; // 所有的点
    int scatterMapWidth = 0;   // 布点地图的宽
    int scatterMapHeight = 0;  // 布点地图的高
};

#endif // SCATTERMAP_H
