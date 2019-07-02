#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

/**
 * 圆的类型
 */
enum class CircleType {
    CenterCircle = QGraphicsEllipseItem::UserType + 1,
    AroundCircle = QGraphicsEllipseItem::UserType + 2,
};

class CenterCircle : public QGraphicsEllipseItem {
public:
    CenterCircle(int n, int radius, const QRectF &rect, QGraphicsItem *parent = nullptr);

    /**
     * 返回类型
     */
    int type() const override { return int(CircleType::CenterCircle); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

private:
    int n;      // 绕大圆的第一圈小圆的个数
    int radius; // 中心大圆的半径
};

/**
 * 小圆，每个圆可以设置
 *     name : 用于查找
 *     value: 显示在圆上
 */
class AroundCircle : public QGraphicsEllipseItem {
public:
    AroundCircle(const QString &value, const QRectF &rect, QGraphicsItem *parent = nullptr);

    /**
     * 返回类型
     */
    int type() const override { return int(CircleType::AroundCircle); }

    /**
     * 设置背景色
     *
     * @param color 背景色
     */
    void setBgcolor(const QString &bgcolor);

    /**
     * 设置 scene 中名字为 name 的 AroundCircle 的 item 的背景色
     *
     * @param scene   场景
     * @param name    圆的名字
     * @param bgcolor 背景色
     */
    static void setAroundCirleBgcolor(QGraphicsScene *scene, const QString &name, const QString &bgcolor);

    /**
     * 重置 scene 中名字为 name 的圆的名字和背景色
     *
     * @param scene 场景
     * @param name  圆的名字
     */
    static void resetAroundCircle(QGraphicsScene *scene, const QString &name);

    /**
     * 返回圆的名字
     */
    QString getName() const { return name; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    /**
     * 重置背景色和设置 name 为空
     */
    void reset();

    bool    hover = false;
    QString name;
    QString value;
    QColor  bgcolor = Qt::transparent;
};

#endif // CIRCLE_H
