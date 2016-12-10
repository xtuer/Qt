#ifndef ARCCHORDPIEWIDGET_H
#define ARCCHORDPIEWIDGET_H

#include <QWidget>

namespace Ui {
class ArcChordPieWidget;
}

/**
 * @brief 画 Arc, Chord and Pie
 */
class ArcChordPieWidget : public QWidget {
    Q_OBJECT

public:
    explicit ArcChordPieWidget(QWidget *parent = 0);
    ~ArcChordPieWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::ArcChordPieWidget *ui;
};

#endif // ARCCHORDPIEWIDGET_H
