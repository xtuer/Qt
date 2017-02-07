#ifndef FILLMODEWIDGET_H
#define FILLMODEWIDGET_H

#include <QWidget>

namespace Ui {
class FillModeWidget;
}

class FillModeWidget : public QWidget {
    Q_OBJECT

public:
    explicit FillModeWidget(QWidget *parent = 0);
    ~FillModeWidget();

private:
    Ui::FillModeWidget *ui;
};

#endif // FILLMODEWIDGET_H
