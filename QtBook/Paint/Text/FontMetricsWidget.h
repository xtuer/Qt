#ifndef FONTMETRICSWIDGET_H
#define FONTMETRICSWIDGET_H

#include <QWidget>

namespace Ui {
class FontMetricsWidget;
}

/**
 * @brief 使用 QFontMetrics 获取字体信息
 */
class FontMetricsWidget : public QWidget {
    Q_OBJECT

public:
    explicit FontMetricsWidget(QWidget *parent = 0);
    ~FontMetricsWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::FontMetricsWidget *ui;
};

#endif // FONTMETRICSWIDGET_H
