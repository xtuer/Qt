#ifndef SELECTABLECHARTVIEW_H
#define SELECTABLECHARTVIEW_H

#include <QChartView>
#include <QList>
#include <QStack>
#include <QPair>

using namespace QtCharts;

class QLabel;
class QAction;

// 坐标轴及其坐标范围
struct AxisRange {
    QAbstractAxis *axis;
    QVariant min;
    QVariant max;
};

class SelectableChartView : public QChartView {
public:
    SelectableChartView(QChart *chart);
    QList<QLabel*> getSelections() const; // 获取所有选区，获取选区的 text，解析得到选择的范围
    void clearSelections(); // 删除所有选区的 Label

    static void handleLegend(QChart *chart);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void createSelection(); // 创建选区
    void createSelectionLabel(QList<AxisRange> ranges, QRect geometry); // 创建选区的 Label
    void setAxisRanges(QList<AxisRange> ranges); // 使用选区缩放坐标轴

    void createContextMenu(); // 创建右键菜单
    void handleLegend();  // 强化 legend 的行为

    QPoint pressedPointAtChart;
    QPoint pressedPointAtChartView;
    QList<QLabel*> selections; // 保存选区的 Label

    QAction *xAction;
    QAction *yAction;
    QAction *xyAction;
    QAction *zoomInAction;
    QAction *zoomResetAction;
    QStack<QList<AxisRange> > zoomStacks;
};

#endif // SELECTABLECHARTVIEW_H
