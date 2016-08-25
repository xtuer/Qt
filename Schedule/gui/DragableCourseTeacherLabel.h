#ifndef DRAGABLECOURSETEACHERLABEL_H
#define DRAGABLECOURSETEACHERLABEL_H

#include <QLabel>

struct DragableCourseTeacherLabelPrivate;

class DragableCourseTeacherLabel : public QLabel {
    Q_OBJECT
public:
    // mimeType 为 Constants::MIMETYPE_COURSE 或者 Constants::MIMETYPE_TEACHER
    explicit DragableCourseTeacherLabel(int id, const QString &name, const QString &mimeType, QWidget *parent = 0);
    ~DragableCourseTeacherLabel();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void startDrag(); // 开始拖拽

    QPoint startPos;
    DragableCourseTeacherLabelPrivate *data;
};

#endif // DRAGABLECOURSETEACHERLABEL_H
