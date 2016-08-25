#ifndef TIMETABLEITEMWIDGET_H
#define TIMETABLEITEMWIDGET_H

#include <QWidget>

class QLabel;
class TimeTableItem;
template <typename T> class QList;

namespace Ui {
class TimeTableItemWidget;
}

class TimeTableItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimeTableItemWidget(QWidget *parent = 0);
    ~TimeTableItemWidget();

    TimeTableItem getTimeTableItem() const;
    void setTimeTableItem(const TimeTableItem &other);

    void setClazz(int clazzId, const QString &name);
    void setCourse(int courseId, const QString &name);
    void setTeacher(int teacherId, const QString &name);
    void showClazz(bool showClazz);
    void showTeacher(bool showTeacher);
    void setEditable(bool editable);

protected:
    virtual bool event(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    void startDrag();
    void updateFlag();
    void updateDragFocus(bool focus);
    void checkConflict();

    Ui::TimeTableItemWidget *ui;
    TimeTableItem *timeTableItem;
    QPoint startPos;
    QLabel *flag;
    bool editable;
};

#endif // TIMETABLEITEMWIDGET_H
