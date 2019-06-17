#ifndef AROUNDCIRCLESWIDGET_H
#define AROUNDCIRCLESWIDGET_H

#include <QWidget>
#include <QList>
#include <QColor>

class AroundCirclesWidget : public QWidget {
    Q_OBJECT

public:
    explicit AroundCirclesWidget(QWidget *parent = nullptr);
    ~AroundCirclesWidget() override;

    /**
     * 计算紧紧围绕半径为 r 的大圆的 n 个小圆的半径 (大圆和小圆相切，小圆之间相切)
     *
     * @param r 大圆的半径
     * @param n 小圆的个数
     * @return 返回小圆的半径
     */
    double aroundCircleRadius(double r, int n) const;

    // 获取第 sn 个小圆的颜色
    QColor getColor(int sn) const;

    // 设置第 sn 个小圆的颜色
    void setColor(int sn, const QColor color);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int     n = 16;       // 大圆上第一圈小圆的个数
    double cr = 100;      // 中心大圆的半径
    QList<QColor> colors; // 每个小圆对应的颜色
};

#endif // AROUNDCIRCLESWIDGET_H
