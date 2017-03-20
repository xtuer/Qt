#ifndef CHECKBOX_H
#define CHECKBOX_H
#include "Widget.h"
#include <QString>
#include <QRect>

class QPainter;

/**
 * CheckBox 类，有选中和未被选中状态。
 */
class CheckBox : public Widget {
public:
    CheckBox(const QString &text, bool checked = true, const QRect &boundingRect = QRect(0, 0, 100, 25));
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
    void mousePressed() Q_DECL_OVERRIDE;

    bool checked; // 选中时为 true，否则为 false
    QString text; // CheckBox 的文本
};

#endif // CHECKBOX_H
