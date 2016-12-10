#ifndef ANTLINEWIDGET_H
#define ANTLINEWIDGET_H

#include <QWidget>
#include <QVector>

class AntLineWidget : public QWidget {
    Q_OBJECT

public:
    explicit AntLineWidget(QWidget *parent = 0);
    ~AntLineWidget();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void advanceDashes();

    int timerId;
    int dashes;
    int spaces;
    const int PATTERN_LENGTH;
    QVector<qreal> dashPattern;
};

#endif // ANTLINEWIDGET_H
