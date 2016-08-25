#include "CourseTeacherWidget.h"
#include "ui_CourseTeacherWidget.h"

#include "bean/Course.h"
#include "bean/Teacher.h"
#include "dao/CourseDao.h"
#include "dao/TeacherDao.h"
#include "Constants.h"

#include "util/SettingsUtil.h"

#include <QList>
#include <QLabel>
#include <QPixmap>
#include <QDrag>
#include <QMimeData>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>

CourseTeacherWidget::CourseTeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseTeacherWidget) {
    ui->setupUi(this);
    this->layout()->setContentsMargins(0, 0, 0, 0);
}

CourseTeacherWidget::~CourseTeacherWidget() {
    SettingsUtil::saveSplitterState(this->objectName(), ui->splitter);
    delete ui;
}

bool CourseTeacherWidget::eventFilter(QObject *watched, QEvent *event) {
    QString className = watched->property(Constants::QSS_CLASS).toString();

    if ((className == Constants::QSS_CLASS_COURSE || Constants::QSS_CLASS_TEACHER)) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                // 记录鼠标按下的位置, 用拖拽开始的计算
                startPos = me->pos();
            }
        } else if (event->type() == QEvent::MouseMove) {
            // 拖拽
            QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
            if (me->buttons() & Qt::LeftButton) {
                int distance = (me->pos() - startPos).manhattanLength();
                if (distance >= QApplication::startDragDistance()) {
                    QLabel *label = qobject_cast<QLabel*>(watched);
                    startDrag(label);
                }
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void CourseTeacherWidget::showEvent(QShowEvent *) {
    loadCourses();
    loadTeachers();
    SettingsUtil::restoreSplitterState(this->objectName(), ui->splitter);
}

void CourseTeacherWidget::loadCourses() {
    // 1. 删除表中已有的widgets.
    QLayout *orgLayout = ui->courseWidget->layout();

    if (orgLayout != NULL) {
        QLayoutItem *child;

        while ((child = orgLayout->itemAt(0)) != NULL) {
            delete child->widget();
        }

        delete orgLayout;
    }

    // 加载课程
    QList<Course*> courses;
    CourseDao::findAllCourses(&courses);
    QGridLayout *layout = new QGridLayout();

    for (int i = 0; i < courses.length(); ++i) {
        Course *c = courses.at(i);
        QLabel *label = new QLabel();
        label->setText(c->name);
        label->setProperty("id", c->id);
        label->setAlignment(Qt::AlignCenter);
        label->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_COURSE);

        label->installEventFilter(this);

        layout->addWidget(label, i / 4, i % 4);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    ui->courseWidget->setLayout(layout);

    qDeleteAll(courses);
}

void CourseTeacherWidget::loadTeachers() {
    // 1. 删除表中已有的widgets.
    QLayout *orgLayout = ui->teacherWidget->layout();

    if (orgLayout != NULL) {
        QLayoutItem *child;

        while ((child = orgLayout->itemAt(0)) != NULL) {
            delete child->widget();
        }

        delete orgLayout;
    }

    // 加载课程
    QList<Teacher*> teachers;
    TeacherDao::findAllTeachers(&teachers);
    QGridLayout *layout = new QGridLayout();

    for (int i = 0; i < teachers.length(); ++i) {
        Teacher *c = teachers.at(i);
        QLabel *label = new QLabel();
        label->setText(c->name);
        label->setProperty("id", c->id);
        label->setAlignment(Qt::AlignCenter);
        label->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_TEACHER);

        label->installEventFilter(this);

        layout->addWidget(label, i / 4, i % 4);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    ui->teacherWidget->setLayout(layout);

    qDeleteAll(teachers);
}

void CourseTeacherWidget::startDrag(QLabel *label) {
    if (NULL == label) { return; }

    int id = label->property("id").toInt();
    QString name = label->text();
    QString className = label->property(Constants::QSS_CLASS).toString();
    QString dataString = QString("%1,%2").arg(id).arg(name);
    QByteArray data = dataString.toUtf8();

    // 开始拖拽
    QMimeData *mimeData = new QMimeData;

    if (Constants::QSS_CLASS_COURSE == className) {
        mimeData->setData(Constants::MIMETYPE_COURSE, data);
    } else if (Constants::QSS_CLASS_TEACHER == className) {
        mimeData->setData(Constants::MIMETYPE_TEACHER, data);
    }

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap = label->grab();
    drag->setPixmap(pixmap);
    QSize s = pixmap.size();
    drag->setHotSpot(QPoint(s.width() / 2, s.height() / 2));
    drag->exec(Qt::MoveAction); // Start drag
}
