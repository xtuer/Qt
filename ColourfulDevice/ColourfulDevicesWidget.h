#ifndef COLOURFULDEVICEWIDGET_H
#define COLOURFULDEVICEWIDGET_H

#include <QWidget>

class GradientColorPicker;

namespace Ui {
class ColourfulDeviceWidget;
}

class ColourfulDeviceWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourfulDeviceWidget(QWidget *parent = nullptr);
    ~ColourfulDeviceWidget();

private:
    void initialize();
    void handleEvents();
    void emptyWidget(QWidget *widget); // 删除 widget 里面的所有子 widget 和 layout

    Ui::ColourfulDeviceWidget *ui;
    GradientColorPicker *colorPicker;
};

#endif // COLOURFULDEVICEWIDGET_H
