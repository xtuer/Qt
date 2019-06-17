#ifndef ARRANGEDEVICESWIDGET_H
#define ARRANGEDEVICESWIDGET_H

#include <QWidget>

class AroundCirclesWidget;

namespace Ui {
class ArrangeDevicesWidget;
}

class ArrangeDevicesWidget : public QWidget {
    Q_OBJECT

public:
    explicit ArrangeDevicesWidget(QWidget *parent = nullptr);
    ~ArrangeDevicesWidget();

private:
    /**
     * @brief 添加设备
     *
     * @param name  设备的名字
     * @param color 设备的颜色，十六进制的颜色，如 #aabbcc
     */
    void addDevice(const QString name, const QString color);

    Ui::ArrangeDevicesWidget *ui;
    AroundCirclesWidget *circlesWidget;
    QList<QString> colors;
};

#endif // ARRANGEDEVICESWIDGET_H
