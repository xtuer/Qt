#ifndef CENTRALTEXTWIDGET_H
#define CENTRALTEXTWIDGET_H

#include <QWidget>

namespace Ui {
class CentralTextWidget;
}

/**
 * @brief 居中绘制文本
 */
class CentralTextWidget : public QWidget {
    Q_OBJECT

public:
    explicit CentralTextWidget(QWidget *parent = 0);
    ~CentralTextWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::CentralTextWidget *ui;
};

#endif // CENTRALTEXTWIDGET_H
