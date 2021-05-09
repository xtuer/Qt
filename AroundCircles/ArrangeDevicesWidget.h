#ifndef ARRANGEDEVICESWIDGET_H
#define ARRANGEDEVICESWIDGET_H

#include <QWidget>
#include <QList>

class QLabel;
class ArrangeDevicesWidgetPrivate;

namespace Ui {
class ArrangeDevicesWidget;
}

class ArrangeDevicesWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * 创建布点的 widget，type 为 1 时创建显示 AroundDevicesGraphicsView，为其他值时显示 PixmapDevicesGraphicsView
     *
     * @param type        布点图的类型
     * @param deviceNames 设备的名字
     * @param parent      父 widget
     */
    explicit ArrangeDevicesWidget(int type, const QStringList &deviceNames, QWidget *parent = nullptr);
    ~ArrangeDevicesWidget() override;

    /**
     * @brief 初始化布点图类型 3 (矩形布点图)
     *
     * @param horizontalCount 水平方向布点个数
     * @param verticalCount   垂直方向布点个数
     */
    void initMode3(int horizontalCount, int verticalCount);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    /**
     * 开始拖拽设备 label
     *
     * @param label 被拖拽的设备 label
     */
    void startDrag(QLabel *label);

    /**
     * 添加设备
     *
     * @param name  设备的名字，唯一
     * @param color 设备的颜色，十六进制的颜色，如 #aabbcc
     */
    void addDevice(const QString name, const QString color);

    Ui::ArrangeDevicesWidget *ui;
    ArrangeDevicesWidgetPrivate *d;
};

#endif // ARRANGEDEVICESWIDGET_H
