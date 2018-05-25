#ifndef AISIGNWIDGET_H
#define AISIGNWIDGET_H

#include <QWidget>

namespace Ui {
class AiSignWidget;
}

class AiSignWidget : public QWidget {
    Q_OBJECT

public:
    explicit AiSignWidget(QWidget *parent = 0);
    ~AiSignWidget();

private:
    Ui::AiSignWidget *ui;
};

#endif // AISIGNWIDGET_H
