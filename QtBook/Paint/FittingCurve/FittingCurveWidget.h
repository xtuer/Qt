#ifndef FITTINGCURVEWIDGET_H
#define FITTINGCURVEWIDGET_H

#include <QWidget>

class FittingCurveWidget : public QWidget {
    Q_OBJECT
public:
    explicit FittingCurveWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // FITTINGCURVEWIDGET_H
