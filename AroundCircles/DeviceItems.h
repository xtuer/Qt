#ifndef DEVICE_ITEMS_H
#define DEVICE_ITEMS_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

/*-----------------------------------------------------------------------------|
 |                                  ItemType                                   |
 |----------------------------------------------------------------------------*/
/**
 * Item 类型
 */
enum class ItemType {
    DIAL_PLATE    = QGraphicsEllipseItem::UserType + 1,
    RECT_DEVICE   = QGraphicsEllipseItem::UserType + 2,
    CIRCLE_DEVICE = QGraphicsEllipseItem::UserType + 3,
};


/*-----------------------------------------------------------------------------|
 |                                  DialPlate                                  |
 |----------------------------------------------------------------------------*/
/**
 * 有刻度的表盘，圆形设备围绕着它进行放置
 */
class DialPlate : public QGraphicsEllipseItem {
public:
    /**
     * 创建有刻度的表盘实例
     *
     * @param n       份数
     * @param radius  半径
     * @param padding 填充
     * @param parent  父 item
     */
    DialPlate(int n, double radius, int padding, QGraphicsItem *parent = nullptr);

    /**
     * 返回类型
     */
    int type() const override { return int(ItemType::DIAL_PLATE); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

private:
    int n;         // 绕大圆的第一圈小圆的个数
    double radius; // 中心大圆的半径
};


/*-----------------------------------------------------------------------------|
 |                                CircleDevice                                 |
 |----------------------------------------------------------------------------*/
/**
 * 圆形设备，每个设备可以设置:
 *     name : 用于查找
 *     value: 显示在圆上
 */
class CircleDevice : public QGraphicsEllipseItem {
public:
    CircleDevice(const QString &value, double radius, QGraphicsItem *parent = nullptr);

    /**
     * 返回 item 类型
     */
    int type() const override { return int(ItemType::CIRCLE_DEVICE); }

    /**
     * 返回设备的名字
     */
    QString getName() const { return name; }

    /**
     * 设置背景色
     *
     * @param color 背景色
     */
    void setBgcolor(const QString &bgcolor);

    /**
     * 设置 scene 中名字为 name 的 CircleDevice 的 item 的背景色
     *
     * @param scene   场景
     * @param name    名字
     * @param bgcolor 背景色
     */
    static void setBgcolorByName(QGraphicsScene *scene, const QString &name, const QString &bgcolor);

    /**
     * 重置背景色和名字
     */
    void reset();

    /**
     * 重置 scene 中名字为 name 的圆的名字和背景色
     *
     * @param scene 场景
     * @param name  名字
     */
    static void resetByName(QGraphicsScene *scene, const QString &name);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    bool    hover = false;
    QString name;
    QString value;
    QColor  bgcolor = Qt::transparent;
};


/*-----------------------------------------------------------------------------|
 |                                 RectDevice                                  |
 |----------------------------------------------------------------------------*/
/**
 * 矩形设备
 */
class RectDevice: public QGraphicsRectItem {
public:
    RectDevice(const QString &name, const QString &value, const QString &bgcolor, const QRectF &rect, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

private:
    QString name;
    QString value;
    QColor  bgcolor = Qt::transparent;
};

#endif // DEVICE_ITEMS_H
