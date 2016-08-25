#include "DragableCourseTeacherLabel.h"

#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QPixmap>
#include <QMimeData>

struct DragableCourseTeacherLabelPrivate {
    int id;
    QString name;
    QString mimeType;
    QPoint startPos;
};

DragableCourseTeacherLabel::DragableCourseTeacherLabel(int id, const QString &name, const QString &mimeType, QWidget *parent)
    : QLabel(parent) {
    data = new DragableCourseTeacherLabelPrivate();
    data->id = id;
    data->name = name;
    data->mimeType = mimeType;

    setText(name);
    setAlignment(Qt::AlignCenter);
}

DragableCourseTeacherLabel::~DragableCourseTeacherLabel() {
    delete data;
}

void DragableCourseTeacherLabel::mousePressEvent(QMouseEvent *event) {
    // 记录鼠标按下的位置, 用拖拽开始的计算
    if (event->button() == Qt::LeftButton) {
        data->startPos = event->pos();
    }
}

void DragableCourseTeacherLabel::mouseMoveEvent(QMouseEvent *event) {
    // 当鼠标距离按下的位置大于可以拖拽的最小距离, 就开始进行拖拽
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - data->startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()) {
            startDrag();
        }
    }
}

void DragableCourseTeacherLabel::startDrag() {
    QString content = QString("%1,%2").arg(data->id).arg(data->name);
    QByteArray contentBytes = content.toUtf8();

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(data->mimeType, contentBytes);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap = grab();
    drag->setPixmap(pixmap);
    QSize s = pixmap.size();
    drag->setHotSpot(QPoint(s.width() / 2, s.height() / 2));
    drag->exec(Qt::MoveAction); // Start drag
}
