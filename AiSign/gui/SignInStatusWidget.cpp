#include "ui_SignInStatusWidget.h"
#include "SignInStatusWidget.h"
#include "bean/Student.h"
#include "bean/SignInInfo.h"
#include "Constants.h"

#include <QDebug>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QToolTip>
#include <QClipboard>
#include <QApplication>
#include <algorithm>

static const int ROLE_NAME       = Qt::UserRole + 1;
static const int ROLE_TOOL_TIP   = Qt::UserRole + 2;
static const int ROLE_EXAM_UID   = Qt::UserRole + 3;
static const int ROLE_ID_CARD_NO = Qt::UserRole + 4;

SignInStatusWidget::SignInStatusWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SignInStatusWidget) {
    initialize();
    handleEvents();
}

SignInStatusWidget::~SignInStatusWidget() {
    delete ui;
}

// 获取已经签到的书生数量
int SignInStatusWidget::signedStudentsCount() const {
    int count = 0;

    foreach (const Student &student, this->students) {
        if (!student.signedAt.isEmpty()) {
            count++;
        }
    }

    return count;
}

void SignInStatusWidget::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 启用 QSS
    onlineIcon = QIcon("image/common/online.png");
    offlineIcon = QIcon("image/common/offline.png");

    model = new QStandardItemModel(this);
    ui->listView->setModel(model);
    ui->listView->setWrapping(true); // 空间不够显示后自动折叠
    ui->listView->setFlow(QListView::LeftToRight);  // 从左向右排列
    ui->listView->setViewMode(QListView::IconMode); // 文字在图标下面
    ui->listView->setResizeMode(QListView::Adjust); // 大小变化后重新布局 items
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
    ui->listView->setIconSize(QSize(36, 36)); // 图标大小
    // ui->listView->setGridSize(QSize(55, 66));

    // 启用鼠标追踪
    ui->listView->setMouseTracking(true);
}

void SignInStatusWidget::handleEvents() {
    // 鼠标放到 item 上时显示提示
    connect(ui->listView, &QListView::entered, [] (const QModelIndex &index) {
        if (!index.isValid()) {
            qDebug() << "Invalid index";
            return;
        }

        QToolTip::showText(QCursor::pos(), index.data(ROLE_TOOL_TIP).toString());
    });

    // 双击复制准考证号
    connect(ui->listView, &QListView::clicked, [] (const QModelIndex &index) {
        if (!index.isValid()) {
            qDebug() << "Invalid index";
            return;
        }

        QString examName = index.data(ROLE_NAME).toString();
        QString examUid = index.data(ROLE_EXAM_UID).toString();
        QApplication::clipboard()->setText(examName + " " + examUid);
        QToolTip::showText(QCursor::pos(), QString("成功复制 %1 的准考证号: %2").arg(examName).arg(examUid));
    });
}

// 查找学生
Student SignInStatusWidget::findStudent(const SignInInfo &info) const {
    for (const Student &student : students) {
        if (SignInMode::SIGN_IN_MANUALLY == info.signInMode && student.examUid == info.examUid) {
            // 人工签到时使用的是学籍号
            return student;
        } else if (student.idCardNo == info.idCardNo) {
            // 其他签到时使用的是身份证号
            return student;
        }
    }

    return Student();
}

void SignInStatusWidget::setStudents(const QList<Student> &students) {
    this->students = students;

    // 按座位排序
    std::sort(this->students.begin(), this->students.end(), [](const Student &a, const Student &b) -> bool {
        return a.seatCode < b.seatCode;
    });

    model->clear();

    foreach (const Student &student, this->students) {
        QIcon icon = student.signedAt.isEmpty() ? offlineIcon : onlineIcon;
        QString text = QString("%1\n%2").arg(student.seatCode).arg(student.examineeName);
        QString toolTip = QString("姓名: %1<br>准考证号: %2<br>身份证号: %3<br>签到时间: %4")
                .arg(student.examineeName).arg(student.examUid)
                .arg(student.idCardNo).arg(student.signedAt);
        QStandardItem *item = new QStandardItem(icon, text);

        item->setData(student.examineeName, ROLE_NAME); // 名字
        item->setData(toolTip, ROLE_TOOL_TIP); // 提示信息
        item->setData(student.examUid, ROLE_EXAM_UID); // 准考证号
        item->setData(student.idCardNo, ROLE_ID_CARD_NO); // 身份证号
        item->setSelectable(false);

        model->appendRow(item);
    }
}

// 签到成功
void SignInStatusWidget::signInSuccess(const SignInInfo &info) {
    qDebug() << info.idCardNo << "签到成功";
    Student signInStudent = findStudent(info);

    // 学生不存在则返回
    if (signInStudent.examUid.isEmpty()) { return; }

    int count = model->rowCount();
    for (int i = 0; i < count; ++i) {
        QStandardItem *item = model->item(i, 0);
        QString examUid  = item->data(ROLE_EXAM_UID).toString();
        QString idCardNo = item->data(ROLE_ID_CARD_NO).toString();

        // 找到签到成功学生的 item，设置其 icon 为在线状态 icon，并更新签到时间到 tool tip
        if (SignInMode::SIGN_IN_MANUALLY == info.signInMode && examUid == info.examUid) {
            QString toolTip = QString("姓名: %1<br>准考证号: %2<br>身份证号: %3<br>签到时间: %4")
                    .arg(signInStudent.examineeName).arg(signInStudent.examUid)
                    .arg(signInStudent.idCardNo).arg(signInStudent.signedAt);

            item->setData(toolTip, ROLE_TOOL_TIP); // 提示信息
            item->setIcon(onlineIcon);

            // find student by examUid
            for (int i = 0; i < students.size(); i++) {
                if (students[i].examUid == info.examUid) {
                    students[i].signedAt = info.signAt;
                    break;
                }
            }
            break;
        } else if (idCardNo.toLower() == info.idCardNo.toLower()) {
            QString toolTip = QString("姓名: %1<br>准考证号: %2<br>身份证号: %3<br>签到时间: %4")
                    .arg(signInStudent.examineeName).arg(signInStudent.examUid)
                    .arg(signInStudent.idCardNo).arg(signInStudent.signedAt);

            item->setData(toolTip, ROLE_TOOL_TIP); // 提示信息
            item->setIcon(onlineIcon);

            // find student by idCardNo
            for (int i = 0; i < students.size(); i++) {
                if (students[i].idCardNo == info.idCardNo) {
                    students[i].signedAt = info.signAt;
                    qDebug() << "goooo";
                    break;
                }
            }
            break;
        }
    }
}
