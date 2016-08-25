#include "TimeTableItemWidget.h"
#include "ui_TimeTableItemWidget.h"

#include "Constants.h"
#include "bean/TimeTableItem.h"
#include "dao/TimeTableItemDao.h"
#include "util/Util.h"
#include "util/UiUtil.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDebug>
#include <QPixmap>
#include <QPixmapCache>

TimeTableItemWidget::TimeTableItemWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::TimeTableItemWidget),
    timeTableItem(new TimeTableItem()), flag(NULL), editable(true) {
    ui->setupUi(this);
    setAcceptDrops(true); // 接受拖放操作

    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->setProperty(Constants::PROPERTY_HAS_TIME_TABLE_ITEM_DATA, true);

    ui->widget->layout()->setSpacing(1);
    ui->widget->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_TIME_TABLE_ITEM);
}

TimeTableItemWidget::~TimeTableItemWidget() {
    delete ui;
    delete timeTableItem;
}

bool TimeTableItemWidget::event(QEvent* event) {
    if (event->type() == QEvent::MouseMove
        || event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::MouseButtonDblClick
        || event->type() == QEvent::Drop
        || event->type() == QEvent::DragEnter) {

        if (!editable) {
            event->accept();
            return true;
        }
    }

    return QWidget::event(event);
}

void TimeTableItemWidget::mousePressEvent(QMouseEvent *event) {
    // 记录鼠标按下的位置, 用拖拽开始的计算
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
}

void TimeTableItemWidget::mouseMoveEvent(QMouseEvent* event) {
    // 当鼠标距离按下的位置大于可以拖拽的最小距离, 就开始进行拖拽
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance() && timeTableItem->editable) {
            startDrag();
        }
    }
}

void TimeTableItemWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->source() == this || !timeTableItem->editable) {
        event->ignore();
        return;
    }

    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasFormat(Constants::MIMETYPE_TIME_TABLE_ITEM)
        || mimeData->hasFormat(Constants::MIMETYPE_COURSE)
        || mimeData->hasFormat(Constants::MIMETYPE_TEACHER)) {
        event->accept();
        updateDragFocus(true);
    } else {
        event->ignore();
    }
}

void TimeTableItemWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    Q_UNUSED(event)
    updateDragFocus(false);
}

void TimeTableItemWidget::dropEvent(QDropEvent *event) {
    if (!timeTableItem->editable) {
        event->ignore();
        return;
    }

    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasFormat(Constants::MIMETYPE_TIME_TABLE_ITEM)) {
        // 交换课表中的 2个 TimeTableItem
        TimeTableItemWidget *source = qobject_cast<TimeTableItemWidget*>(event->source());

        TimeTableItem tti1(this->getTimeTableItem());
        TimeTableItem tti2(source->getTimeTableItem());
        Util::swapTimeTableItem(&tti1, &tti2);

        this->setTimeTableItem(tti1);
        source->setTimeTableItem(tti2);

        event->accept();
        updateDragFocus(false);
    } else if (mimeData->hasFormat(Constants::MIMETYPE_COURSE)) {
        QStringList data = QString::fromUtf8(mimeData->data(Constants::MIMETYPE_COURSE)).split(",");
        int id = data.at(0).toInt();
        QString name = data.at(1);
        setCourse(id, name);

        event->accept();
        updateDragFocus(false);
    } else if (mimeData->hasFormat(Constants::MIMETYPE_TEACHER)) {
        QStringList data = QString::fromUtf8(mimeData->data(Constants::MIMETYPE_TEACHER)).split(",");
        int id = data.at(0).toInt();
        QString name = data.at(1);
        setTeacher(id, name);

        event->accept();
        updateDragFocus(false);
    }
}

void TimeTableItemWidget::mouseDoubleClickEvent(QMouseEvent*) {
    timeTableItem->editable = !timeTableItem->editable;
    updateFlag();
}

void TimeTableItemWidget::startDrag() {
    // 开始拖拽
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(Constants::MIMETYPE_TIME_TABLE_ITEM, QByteArray());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap = this->grab();
    drag->setPixmap(pixmap);
    QSize s = pixmap.size();
    drag->setHotSpot(QPoint(s.width() / 2, s.height() / 2));
    drag->exec(Qt::MoveAction); // Start drag
}

void TimeTableItemWidget::updateFlag() {
    // 创建 flag label
    if (NULL == flag && !timeTableItem->editable) {
        QPixmap pm;
        if (!QPixmapCache::find(Constants::FLAG_IMAGE_NAME, &pm)) {
            pm.load(Constants::FLAG_IMAGE_PATH);
            QPixmapCache::insert(Constants::FLAG_IMAGE_NAME, pm);
        }
        flag = new QLabel(this);
        flag->setPixmap(pm);
    }

    if (flag != NULL) {
        flag->setVisible(!timeTableItem->editable);
    }
}

void TimeTableItemWidget::updateDragFocus(bool focus) {
    ui->widget->setProperty(Constants::PROPERTY_DRAG_FOCUS, focus);
    UiUtil::updateStyleSheet(ui->widget);
}

TimeTableItem TimeTableItemWidget::getTimeTableItem() const {
    timeTableItem->row = property(Constants::PROPERTY_ROW).toInt();
    timeTableItem->col = property(Constants::PROPERTY_COLUMN).toInt();

    return *timeTableItem;
}

void TimeTableItemWidget::setTimeTableItem(const TimeTableItem& other) {
    delete timeTableItem;
    timeTableItem = new TimeTableItem(other);

    // 设置后在界面上显示出来
    ui->classLabel->setText(timeTableItem->clazzName);
    ui->courseLabel->setText(timeTableItem->courseName);
    ui->teacherLabel->setText(timeTableItem->teacherName);

    updateFlag();
    checkConflict();
}

void TimeTableItemWidget::setClazz(int clazzId, const QString &name) {
    TimeTableItem item(*this->timeTableItem);
    item.clazzId = clazzId;
    item.clazzName = name;

    this->setTimeTableItem(item);
}

void TimeTableItemWidget::setCourse(int courseId, const QString &name) {
    TimeTableItem item(*this->timeTableItem);
    item.courseId = courseId;
    item.courseName = name;

    this->setTimeTableItem(item);
}

void TimeTableItemWidget::setTeacher(int teacherId, const QString &name) {
    TimeTableItem item(*this->timeTableItem);
    item.teacherId = teacherId;
    item.teacherName = name;

    this->setTimeTableItem(item);
}

void TimeTableItemWidget::showClazz(bool showClazz) {
    ui->classLabel->setVisible(showClazz);
}

void TimeTableItemWidget::showTeacher(bool showTeacher) {
    ui->teacherLabel->setVisible(showTeacher);
}

void TimeTableItemWidget::setEditable(bool editable) {
    this->editable = editable;
}

void TimeTableItemWidget::checkConflict() {
    bool conflict = false;

    if (timeTableItem->teacherId > 0) {
        int count = 0;
        TimeTableItemDao::otherTimeTableItemCountAtTheSameTime(*timeTableItem, &count);
        conflict = count > 0;
    }

    ui->widget->setProperty(Constants::PROPERTY_TIME_TABLE_ITEM_CONFLICT, conflict);
    UiUtil::updateStyleSheet(ui->widget);
}
