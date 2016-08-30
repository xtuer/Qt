#ifndef SCHEDULEITEMWIDGET_H
#define SCHEDULEITEMWIDGET_H

#include "bean/ScheduleItem.h"
#include <QWidget>
#include <QLabel>

namespace Ui {
class ScheduleItemWidget;
}

/**
 * @brief 课程表项的 widget，上面显示课程，老师，班级以及是否可编辑
 */
class ScheduleItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScheduleItemWidget(ScheduleItem scheduleItem, bool classVisible, bool teacherVisible, QWidget *parent = 0);
    ~ScheduleItemWidget();

    void setScheduleItem(const ScheduleItem &item, bool checkConflict = true);
    ScheduleItem getScheduleItem() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
    void startDrag(); // 开始拖拽
    void updateDragFocus(bool focus); // 拖拽进入和离开时，更行焦点样式
    void updateConflict(bool conflict); // 更新冲突的样式

    Ui::ScheduleItemWidget *ui;
    ScheduleItem scheduleItem;
    QLabel *editableLabel;
    bool classVisible;
    bool teacherVisible;
    QPoint startPos;
};

#endif // SCHEDULEITEMWIDGET_H
