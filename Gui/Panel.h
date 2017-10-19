#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class Panel : public QWidget {
public:
    Panel(QWidget *parent = NULL);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // PANEL_H
