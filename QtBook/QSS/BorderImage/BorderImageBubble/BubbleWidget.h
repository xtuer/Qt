#ifndef BUBBLEWIDGET_H
#define BUBBLEWIDGET_H

#include <QWidget>

namespace Ui {
class BubbleWidget;
}

class BubbleWidget : public QWidget {
    Q_OBJECT

public:
    explicit BubbleWidget(QWidget *parent = 0);
    ~BubbleWidget();

private:
    Ui::BubbleWidget *ui;
};

#endif // BUBBLEWIDGET_H
