#include "ScheduleItemWidget.h"
#include "ui_ScheduleItemWidget.h"
#include "Constants.h"
#include "util/Util.h"
#include "util/UiUtil.h"
#include "dao/ScheduleDao.h"

#include <QMimeData>
#include <QDrag>
#include <QPixmap>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QDragEnterEvent>

ScheduleItemWidget::ScheduleItemWidget(ScheduleItem scheduleItem, bool classVisible, bool teacherVisible, QWidget *parent) :
    QWidget(parent), ui(new Ui::ScheduleItemWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效
    setAcceptDrops(true); // 接受拖放操作

    // 创建显示可编辑状态的 Label
    this->editableLabel = new QLabel(this);
    this->editableLabel->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_SCHEDULE_ITEM_EDITABLE_LABEL);
    this->editableLabel->move(0, 0);

    // 默认老师和课程都不显示
    this->classVisible = classVisible;
    this->teacherVisible = teacherVisible;

    setScheduleItem(scheduleItem, false);
}

ScheduleItemWidget::~ScheduleItemWidget() {
    delete ui;
}


void ScheduleItemWidget::setScheduleItem(const ScheduleItem &item, bool checkConflict) {
    scheduleItem = item;
    ui->classLabel->setVisible(classVisible);
    ui->teacherLabel->setVisible(teacherVisible);
    ui->classLabel->setText(scheduleItem.className);
    ui->courseLabel->setText(scheduleItem.courseName);
    ui->teacherLabel->setText(scheduleItem.teacherName);

    if (checkConflict) {
        scheduleItem.conflict = ScheduleDao::isScheduleItemConflict(scheduleItem);
        updateConflict(scheduleItem.conflict);
    }

    editableLabel->setVisible(!scheduleItem.editable); // 不可编辑时显示不可编辑的label
}

ScheduleItem ScheduleItemWidget::getScheduleItem() const {
    return scheduleItem;
}

void ScheduleItemWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event)

    // 双击时改变可编辑状态
    scheduleItem.editable = !scheduleItem.editable;
    this->editableLabel->setVisible(!scheduleItem.editable);
}

void ScheduleItemWidget::mousePressEvent(QMouseEvent *event) {
    // 记录鼠标按下的位置, 用拖拽开始的计算
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
}

void ScheduleItemWidget::mouseMoveEvent(QMouseEvent *event) {
    // 当鼠标距离按下的位置大于可以拖拽的最小距离, 就开始进行拖拽
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance() && scheduleItem.editable) {
            startDrag();
        }
    }
}

void ScheduleItemWidget::dragEnterEvent(QDragEnterEvent *event) {
    // 如果是自己，或者不可编辑的话，不接受拖拽进入事件
    if (event->source() == this || !scheduleItem.editable) {
        event->ignore();
        return;
    }

    const QMimeData *mimeData = event->mimeData();

    // 只接受指定类型拖拽的数据
    if (mimeData->hasFormat(Constants::MIME_TYPE_SCHEDULE_ITEM)
        || mimeData->hasFormat(Constants::MIME_TYPE_COURSE)
        || mimeData->hasFormat(Constants::MIME_TYPE_TEACHER)) {
        event->accept();
        updateDragFocus(true);
    } else {
        event->ignore();
    }
}

void ScheduleItemWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    Q_UNUSED(event)
    updateDragFocus(false);
}

void ScheduleItemWidget::dropEvent(QDropEvent *event) {
    if (!scheduleItem.editable) {
        event->ignore();
        return;
    }

    const QMimeData *mimeData = event->mimeData(); // 获得 drop 的数据

    if (mimeData->hasFormat(Constants::MIME_TYPE_SCHEDULE_ITEM)) {
        // 交换课表中的 2 个 ScheduleItemWidget
        ScheduleItemWidget *source = qobject_cast<ScheduleItemWidget*>(event->source());
        ScheduleItemWidget *target = this;

        ScheduleItem item1(target->getScheduleItem());
        ScheduleItem item2(source->getScheduleItem());
        Util::exchangeScheduleItemDayAndTime(&item1, &item2);

        // 如果不交换 day 和 time，则显示的位置不对
        target->setScheduleItem(item2);
        source->setScheduleItem(item1);

        event->accept();
        updateDragFocus(false);
    } else if (mimeData->hasFormat(Constants::MIME_TYPE_COURSE)) {
        QStringList data = QString::fromUtf8(mimeData->data(Constants::MIME_TYPE_COURSE)).split(",");
        int courseId = data.at(0).toInt();
        QString courseName = data.at(1);

        ScheduleItem item(this->getScheduleItem());
        item.courseId = courseId;
        item.courseName = courseName;
        this->setScheduleItem(item);

        event->accept();
        updateDragFocus(false);
    } else if (mimeData->hasFormat(Constants::MIME_TYPE_TEACHER)) {
        QStringList data = QString::fromUtf8(mimeData->data(Constants::MIME_TYPE_TEACHER)).split(",");
        int teacherId = data.at(0).toInt();
        QString teacherName = data.at(1);

        ScheduleItem item(this->getScheduleItem());
        item.teacherId = teacherId;
        item.teacherName = teacherName;
        this->setScheduleItem(item);

        event->accept();
        updateDragFocus(false);
    }
}

void ScheduleItemWidget::startDrag() {
    // 开始拖拽，设置 mima 类型和数据
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(Constants::MIME_TYPE_SCHEDULE_ITEM, QByteArray());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap = this->grab();
    drag->setPixmap(pixmap);
    QSize s = pixmap.size();
    drag->setHotSpot(QPoint(s.width() / 2, s.height() / 2));
    drag->exec(Qt::MoveAction); // Start drag
}

void ScheduleItemWidget::updateDragFocus(bool focus) {
    this->setProperty(Constants::PROPERTY_DRAG_FOCUS, focus);
    UiUtil::updateQss(this);
}

void ScheduleItemWidget::updateConflict(bool conflict) {
    this->setProperty(Constants::PROPERTY_SCHEDULE_ITEM_CONFLICT, conflict);
    UiUtil::updateQss(this);
}
