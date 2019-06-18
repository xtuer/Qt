#ifndef ARRANGEDEVICESWIDGET_H
#define ARRANGEDEVICESWIDGET_H

#include <QWidget>

class QLabel;
class ArrangeDevicesWidgetPrivate;

namespace Ui {
class ArrangeDevicesWidget;
}

class ArrangeDevicesWidget : public QWidget {
    Q_OBJECT

public:
    explicit ArrangeDevicesWidget(QWidget *parent = nullptr);
    ~ArrangeDevicesWidget() override;

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
