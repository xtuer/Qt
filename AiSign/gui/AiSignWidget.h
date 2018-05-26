#ifndef AISIGNWIDGET_H
#define AISIGNWIDGET_H

#include <QWidget>

struct AiSignWidgetPrivate;

namespace Ui {
class AiSignWidget;
}

class AiSignWidget : public QWidget {
    Q_OBJECT

public:
    explicit AiSignWidget(QWidget *parent = 0);
    ~AiSignWidget();

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理

    Ui::AiSignWidget *ui;
    AiSignWidgetPrivate *d;

    friend struct AiSignWidgetPrivate;
};

#endif // AISIGNWIDGET_H
