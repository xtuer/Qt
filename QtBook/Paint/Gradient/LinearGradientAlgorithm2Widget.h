#ifndef LINEARGRADIENTALGORITHM2WIDGET_H
#define LINEARGRADIENTALGORITHM2WIDGET_H

#include <QWidget>

namespace Ui {
class LinearGradientAlgorithm2Widget;
}

class LinearGradientAlgorithm2Widget : public QWidget {
    Q_OBJECT

public:
    explicit LinearGradientAlgorithm2Widget(QWidget *parent = 0);
    ~LinearGradientAlgorithm2Widget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::LinearGradientAlgorithm2Widget *ui;
};

#endif // LINEARGRADIENTALGORITHM2WIDGET_H
