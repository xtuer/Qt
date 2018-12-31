#ifndef SELECTABLECHARTVIEW_H
#define SELECTABLECHARTVIEW_H

#include <QChartView>
#include <QList>
#include <QStack>
#include <QPair>
#include <QHash>
#include <QDateTime>

using namespace QtCharts;
namespace QtCharts {
    class QDateTimeAxis;
    class QValueAxis;
}

class QLabel;
class QAction;

// 坐标轴及其坐标范围
struct AxisRange {
    QAbstractAxis *axis;
    QVariant min;
    QVariant max;
};

// 记录器校准的范围
struct CalibrationRange {
    QDateTime minTime;
    QDateTime maxTime;
    double minTemperature;
    double maxTemperature;
};

class SelectableChartView : public QChartView {
public:
    SelectableChartView(QChart *chart);
    QList<QLabel*> getSelections() const; // 获取所有选区，获取选区的 text，解析得到选择的范围
    void clearSelections(); // 删除所有选区的 Label

    // 获取水平时间轴的最小值
    QDateTime getMinDateTime() const;

    // 获取水平时间轴的最大值
    QDateTime getMaxDateTime() const;

    // 获取左边垂直坐标轴的最小温度
    double getMinTemperature() const;

    // 获取左边垂直坐标轴的最高温度
    double getMaxTemperature() const;

    // 获取水平的时间坐标轴
    QDateTimeAxis* getDateTimeAxis() const;

    // 获取左边垂直坐标轴的温度坐标轴
    QValueAxis* getTemperatureAxis() const;

    // 设置灭菌参数
    void setSterilizationParams(const QDateTime &sterilizeStartTime, // 灭菌开始时间
                                const QDateTime &sterilizeEndTime, // 灭菌结束时间
                                double sterilizeTemperature); // 灭菌温度
    // 设置是否显示灭菌的辅助线
    void setSterilizationMarkersVisible(bool visible);

    static void handleLegend(QChart *chart);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void createSelection(); // 创建选区
    void createSelectionLabel(QList<AxisRange> ranges, QRect geometry); // 创建选区的 Label
    void setAxisRanges(QList<AxisRange> ranges); // 使用选区缩放坐标轴

    void createContextMenu(); // 创建右键菜单
    void handleLegend();  // 强化 legend 的行为
    void drawSterilizationMarkers(); // 绘制灭菌辅助线

    QPoint pressedPointAtChart;
    QPoint pressedPointAtChartView;
    QList<QLabel*> selectionLabels; // 保存选区的 Label
    QHash<QLabel*, CalibrationRange> calibrationRanges; // 选区的数据

    QAction *xAction;
    QAction *yAction;
    QAction *xyAction;
    QAction *zoomInAction;
    QAction *zoomResetAction;
    QAction *calibrationAction; // 记录器校准
    QStack<QList<AxisRange> > zoomStacks;

    QWidget *sterilizationMarkerWidget; // 灭菌辅助线 widget
    QDateTime sterilizeStartTime; // 灭菌开始时间
    QDateTime sterilizeEndTime;   // 灭菌结束时间
    double sterilizeTemperature;   // 灭菌温度
};

#endif // SELECTABLECHARTVIEW_H
