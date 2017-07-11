#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QList>
#include <QPainterPath>

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QList<QPointF> points1; // 曲线一的顶点数组
    QList<QPointF> points2; // 曲线二的顶点数组
    QPainterPath smoothCurvePath1; // 平滑曲线一
    QPainterPath smoothCurvePath2; // 平滑曲线二
};

#endif // FORM_H
