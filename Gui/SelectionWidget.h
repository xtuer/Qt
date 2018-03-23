#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include <QWidget>

class SelectionWidget : public QWidget {
public:
    SelectionWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // SELECTIONWIDGET_H
