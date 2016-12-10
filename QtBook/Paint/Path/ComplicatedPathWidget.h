#ifndef COMPLICATEDPATHWIDGET_H
#define COMPLICATEDPATHWIDGET_H

#include <QWidget>

namespace Ui {
class ComplicatedPathWidget;
}

class ComplicatedPathWidget : public QWidget {
    Q_OBJECT

public:
    explicit ComplicatedPathWidget(QWidget *parent = 0);
    ~ComplicatedPathWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::ComplicatedPathWidget *ui;
};

#endif // COMPLICATEDPATHWIDGET_H
