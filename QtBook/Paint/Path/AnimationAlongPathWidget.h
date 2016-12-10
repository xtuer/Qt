#ifndef ANIMATIONALONGPATHWIDGET_H
#define ANIMATIONALONGPATHWIDGET_H

#include <QWidget>
#include <QPainterPath>

class AnimationAlongPathWidget : public QWidget {
    Q_OBJECT

public:
    explicit AnimationAlongPathWidget(QWidget *parent = 0);
    ~AnimationAlongPathWidget();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPainterPath path;
    float percent;
    float step;
    int timerId;
};

#endif // ANIMATIONALONGPATHWIDGET_H
