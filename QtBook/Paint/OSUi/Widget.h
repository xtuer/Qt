#ifndef WIDGET_H
#define WIDGET_H
#include <QRect>

class QPainter;

/**
 * 控件的基类 Widget，所有控件类都需要直接或者间接的继承类 Widget，它处理控件类的一些共有的逻辑。
 */
class Widget {
public:
    Widget(const QRect &boundingRect);

    // 绘制控件的函数，不同的控件提供不同的实现，绘制出不同的样子。
    // paint() 是纯虚函数，控件子类必须提供它的实现。
    virtual void paint(QPainter *painter) = 0;

    // 一下几个为鼠标事件
    virtual void mouseMove();     // 鼠标移动事件
    virtual void mouseEnter();    // 鼠标进入事件
    virtual void mouseLeave();    // 鼠标离开事件
    virtual void mousePressed();  // 鼠标按下事件
    virtual void mouseReleased(); // 鼠标按下后松开事件

    bool  hover;        // 鼠标移动到 widget 上时为 true，其他时候为 false
    bool  pressed;      // 鼠标在按住 widget 时为 true，其他时候为 false
    QRect boundingRect; // Widget 的范围: 左上角的坐标和宽、高
};

#endif // WIDGET_H
