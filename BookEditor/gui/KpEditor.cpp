#include "ui_KpEditor.h"
#include "KpEditor.h"
#include "MessageBox.h"
#include "bean/Book.h"
#include "util/UiUtil.h"
#include "service/Service.h"
#include "service/KpService.h"
#include "service/MergeService.h"
#include "delegate/KpDelegate.h"

#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QTreeView>
#include <QHeaderView>
#include <QShortcut>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

KpEditor::KpEditor(bool readOnly, QWidget *parent) : QWidget(parent), ui(new Ui::KpEditor), readOnly(readOnly) {
    initialize();
    handleEvents();

    openSubjects();
}

KpEditor::~KpEditor() {
    qDebug() << "~KpEditor";
    delete ui;
}

// 只读模式时点击了 Ok 按钮
bool KpEditor::isOkButtonClickedInReadOnlyMode() const {
    return okButtonClickedInReadOnlyMode;
}

/**
 * 获取选中的知识点信息，放在一个 QStringList 中
 * 0: 知识点名字
 * 1: 知识点编码
 * 2: 知识点所属学科编码
 *
 * @return 返回知识点信息的列表
 */
QStringList KpEditor::getSelectedKp() const {
    QModelIndex current = UiUtil::getSelectedIndex(ui->kpsTreeView);

    if (current.isValid()) {
        QModelIndex parent  = current.parent();
        QString name = kpsModel->index(current.row(), 0, parent).data().toString();
        QString code = kpsModel->index(current.row(), 1, parent).data().toString();

        return { name,  code, subjectCode };
    } else {
        return {};
    }
}

bool KpEditor::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->messageLabel && event->type() == QEvent::Show) {
        QTimer::singleShot(2000, ui->messageLabel, SLOT(hide()));
    }

    return QWidget::eventFilter(watched, event);
}

// 初始化
void KpEditor::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->messageLabel->hide();
    ui->messageLabel->installEventFilter(this);

    if (readOnly) {
        ui->reopenButton->hide();
        ui->checkButton->hide();
        ui->saveButton->hide();
        ui->subjectsTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->kpsTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    } else {
        ui->okButton->hide();
    }

    // 创建 kps 文件夹
    kpsDir = QDir("data/kps");
    if (!kpsDir.exists()) {
        kpsDir.mkpath(".");
    }

    // 左侧学科的树
    subjectsModel = new QStandardItemModel(this);
    subjectsModel->setHorizontalHeaderLabels({ "学段 > 学科" });
    ui->subjectsTreeView->setModel(subjectsModel);
    ui->subjectsTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑

    // 知识点的树
    kpsModel = new QStandardItemModel(this);
    kpsModel->setHorizontalHeaderLabels({ "知识点", "编码", "认知发展(必修)", "认知发展(选择性必修)", "学业质量参考(学业考)", "学业质量参考(等级考)" });
    ui->kpsTreeView->setModel(kpsModel);
    ui->kpsTreeView->hideColumn(1);
    ui->kpsTreeView->header()->setSectionsMovable(false);
//    ui->kpsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    ui->kpsTreeView->setColumnWidth(0, 300);
    ui->kpsTreeView->setColumnWidth(2, 150);
    ui->kpsTreeView->setColumnWidth(3, 150);
    ui->kpsTreeView->setColumnWidth(4, 150);
    ui->kpsTreeView->setColumnWidth(5, 150);

    ui->kpsTreeView->setItemDelegate(new KpDelegate(ui->subjectsTreeView->selectionModel(), this));

    // createSubjectsContextMenu(); // 创建左侧学科的右键菜单
    createKpsContextMenu();      // 创建中间知识点右键菜单

    kpService = new KpService(subjectsModel, kpsModel, kpsDir);
}

// 事件处理
void KpEditor::handleEvents() {
    // 学科树的当前节点变化时，如果是学科类型的话打开对应的知识点
    connect(ui->subjectsTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this] (const QModelIndex &index) {
        resetKps();

        if (Service::isSubjectIndex(index)) {
            subjectCode = index.data(ROLE_CODE).toString();
            subjectName = index.data().toString();
            openSubjectKps(subjectCode); // 打开学科的知识点
        }
    });

    // 学科、知识点节点的内容变化后自动保存
    connect(subjectsModel, &QStandardItemModel::itemChanged, [this] () {
        ui->saveButton->click();
    });
    connect(kpsModel, &QStandardItemModel::itemChanged, [this] () {
        ui->saveButton->click();
    });

    // 校验
    connect(ui->checkButton, &QPushButton::clicked, [this] {
        if (validate()) {
            UiUtil::showMessage(ui->messageLabel, "验证通过");
        }
    });

    // 保存
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        save();
    });

    // 重新加载
    connect(ui->reopenButton, &QPushButton::clicked, [this] {
        openSubjects();
    });

    if (readOnly) {
        connect(ui->okButton, &QPushButton::clicked, [this] {
            okButtonClickedInReadOnlyMode = true;
            UiUtil::findWindow(this)->close();
        });

        connect(ui->kpsTreeView, &QTreeView::doubleClicked, [this] (const QModelIndex &index) {
            if (index.isValid()) {
                ui->okButton->click();
            }
        });
    }

    QShortcut *mergeShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_K), this);
    QObject::connect(mergeShortcut, &QShortcut::activated, [] {
        MergeService ms;
        Q_UNUSED(ms)
        MessageBox::message("合并完成");
    });
}

// 创建左侧学科的右键菜单
void KpEditor::createSubjectsContextMenu() {
    // 左侧教材树的右键菜单: 学段 > 学科 > 教材
    QAction *phaseAction    = new QAction("新建学段", this);
    QAction *subjectAction  = new QAction("新建学科", this);
    QAction *deleteAction   = new QAction("删除操作", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->subjectsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧学科树节点，弹出右键菜单
    connect(ui->subjectsTreeView, &QWidget::customContextMenuRequested, [=]() {
        // 根据 index 类型判断要显示的右键菜单项
        // 1. 点击学段显示新建学科
        // 2. 点击空白显示新建学段

        QMenu menu;
        QModelIndex index = UiUtil::indexAt(ui->subjectsTreeView, QCursor::pos());
        leftClickedLeftIndex = index;

        if (Service::isPhaseIndex(index)) {
            menu.addAction(subjectAction); // [1] 显示新建学科
            menu.addSeparator();
            deleteAction->setText("删除学段");
            menu.addAction(deleteAction);
        } else if (Service::isSubjectIndex(index)) {
            deleteAction->setText("删除学科");
            menu.addAction(deleteAction);
        } else {
            menu.addAction(phaseAction); // [2] 显示新建学段
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 创建新的学段
    connect(phaseAction, &QAction::triggered, [this] {
        Service::appendRow(subjectsModel, QModelIndex(), { Service::createPhaseItem("新建学段") });
    });

    // 创建新的学科
    connect(subjectAction, &QAction::triggered, [this] {
        if (!Service::isPhaseIndex(leftClickedLeftIndex)) { return; }

        Service::appendRow(subjectsModel, leftClickedLeftIndex, { Service::createSubjectItem("新建学科") });
        ui->subjectsTreeView->expand(leftClickedLeftIndex);
    });

    // 删除操作
    connect(deleteAction, &QAction::triggered, [this] {
        if (!currentLeftIndex().isValid()) { return; }

        QString name = currentLeftIndex().data().toString();
        int rowCount = subjectsModel->itemFromIndex(currentLeftIndex())->rowCount();

        if (Service::isPhaseIndex(currentLeftIndex())) {
            // 删除学段
            // 学段下还有学科时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有学科，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除学段 <font color='darkred'>%1</font> 吗?").arg(name))) {
                subjectsModel->removeRow(currentLeftIndex().row(), currentLeftIndex().parent());
            }
        } else if (Service::isSubjectIndex(currentLeftIndex())) {
            // 删除学科
            if (MessageBox::confirm(QString("确定要删除学科 <font color='darkred'>%1</font> 吗?").arg(name))) {
                // 删除此学科的知识点文件
                QString subjectCode = currentLeftIndex().data(ROLE_CODE).toString().trimmed();
                QFile::remove(Service::subjectKpsFilePath(kpsDir, subjectCode));

                // 从树中删除学科节点
                subjectsModel->removeRow(currentLeftIndex().row(), currentLeftIndex().parent());

                // 删除的时候保存一下
                ui->saveButton->click();
            }
        }
    });

    // 全部展开教材树
    connect(expandAction, &QAction::triggered, [this] {
        ui->subjectsTreeView->expandAll();
    });

    // 全部收拢教材树
    connect(collapseAction, &QAction::triggered, [this] {
        ui->subjectsTreeView->collapseAll();
    });
}

// 创建中间知识点右键菜单
void KpEditor::createKpsContextMenu() {
    QAction *appendChildKpAction = new QAction("添加子知识点", this);
    QAction *insertBeforeAction  = new QAction("插入前一知识点", this);
    QAction *insertAfterAction   = new QAction("插入后一知识点", this);
    QAction *deleteAction   = new QAction("删除", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->kpsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧学科树节点，弹出右键菜单
    connect(ui->kpsTreeView, &QWidget::customContextMenuRequested, [=]() {
        if (!Service::isSubjectIndex(currentLeftIndex())) {
            return;
        }

        QMenu menu;
        rightClickedKpIndex = UiUtil::indexAt(ui->kpsTreeView, QCursor::pos());

        // 知识点的章节节点也不弹出菜单
        if (Service::isChapterIndex(rightClickedKpIndex)) {
            return;
        }

        menu.addAction(appendChildKpAction);

        if (rightClickedKpIndex.isValid()) {
            menu.addSeparator();
            menu.addAction(insertBeforeAction);
            menu.addAction(insertAfterAction);
            menu.addSeparator();
            menu.addAction(deleteAction);
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 添加子知识点
    connect(appendChildKpAction, &QAction::triggered, [this] {
        kpService->appendChildKp(rightClickedKpIndex, subjectCode);
        ui->kpsTreeView->expand(rightClickedKpIndex);
    });

    // 插入前一知识点
    connect(insertBeforeAction, &QAction::triggered, [this] {
        kpService->insertKp(rightClickedKpIndex, true, subjectCode);
        ui->kpsTreeView->expand(rightClickedKpIndex);
    });

    // 插入后一知识点
    connect(insertAfterAction, &QAction::triggered, [this] {
        kpService->insertKp(rightClickedKpIndex, false, subjectCode);
        ui->kpsTreeView->expand(rightClickedKpIndex);
    });

    // 删除知识点
    connect(deleteAction, &QAction::triggered, [this] {
        if (!rightClickedKpIndex.isValid()) { return; }

        QModelIndex nameIndex = kpsModel->index(rightClickedKpIndex.row(), 0, rightClickedKpIndex.parent());
        QString name = nameIndex.data().toString();

        // 如果有子知识点，不让删除，避免误删除
        if (kpsModel->rowCount(nameIndex) > 0) {
            MessageBox::message(QString("<font color='darkred'>%1</font> 下还有知识点，不能删除").arg(name));
        } else if (MessageBox::confirm(QString("确定要删除 <font color='darkred'>%1</font> 吗?").arg(name))) {
            kpsModel->removeRow(nameIndex.row(), nameIndex.parent());
        }
    });

    // 全部展开知识点树
    connect(expandAction, &QAction::triggered, [this] {
        ui->kpsTreeView->expandAll();
    });

    // 全部收拢知识点树
    connect(collapseAction, &QAction::triggered, [this] {
        ui->kpsTreeView->collapseAll();
    });
}

// 重置知识点
void KpEditor::resetKps() {
    kpsModel->removeRows(0, kpsModel->rowCount());
}

// 打开学科到左侧的教材目录树中
void KpEditor::openSubjects() {
    resetKps();
    kpService->readSubjects();
    ui->subjectsTreeView->expandAll();
}

// 打开学科的知识点到右侧的知识点树中
void KpEditor::openSubjectKps(const QString &subjectCode) {
    kpService->readSubjectKps(subjectCode, !readOnly);
    ui->kpsTreeView->expandAll();
}

// 校验学科和知识点的编码是否唯一，未被重复使用
bool KpEditor::validate() const {
    // 校验逻辑:
    // 1. 没选择学科则只校验学科结构
    // 2. 选择学科不但要校验学科结构，还要校验学科的知识点
    //    2.1 学科的名字不能为空
    //    2.2 学科的编码为空且知识点为空，则返回 true
    //    2.3 学科的编码为空且知识点不为空，则返回 false
    //    2.4 学科编码不能重复使用
    //    2.5 知识点编码不能在当前学科中重复使用

    if (Service::isSubjectIndex(currentLeftIndex())) {
        // [2] 选择学科不但要校验学科结构，还要校验学科的知识点

        // [2.4] 学科编码不能重复使用
        // [2.5] 知识点编码不能在当前学科中重复使用
        QString error2;
        bool ok2 = kpService->validateSubjectKps(&error2);

        if (!ok2) {
            QString error = QString("<center><font color='red'>校验失败: 编码被重复使用</font></center><br>%1").arg(error2);

            MessageBox::message(error, 400, 300);
        }

        return ok2;
    }

    return true;

    /*if (!Service::isSubjectIndex(currentLeftIndex())) {
        // [1] 没选择学科则只校验学科结构
        QString error;
        bool ok = kpService->validateSubjects(currentLeftIndex(), ui->kpCodeEdit->text().trimmed(), &error);

        if (!ok) {
            error.replace("\n", "<br>");
            error = QString("<center><font color='red'>校验失败:</font></center><br>%1").arg(error);

            MessageBox::message(error, 400, 300);
        }

        return ok;
    } else {
        // [2] 选择学科不但要校验学科结构，还要校验学科的知识点
        // [2.1] 学科的名字不能为空
        if (ui->kpSubjectEdit->text().trimmed().isEmpty()) {
            MessageBox::message("<font color='red'>学科</font> 不能为空");
            return false;
        }

        // [2.2] 学科的编码为空且知识点为空，则返回 true
        if (ui->kpCodeEdit->text().trimmed().isEmpty() && kpsModel->rowCount() == 0) {
            return true;
        }

        // [2.3] 学科的编码为空且知识点不为空，则返回 false
        if (ui->kpCodeEdit->text().trimmed().isEmpty() && kpsModel->rowCount() > 0) {
            MessageBox::message("学科的 <font color='red'>编码</font> 不能为空");
            return false;
        }

        // [2.4] 学科编码不能重复使用
        // [2.5] 知识点编码不能在当前学科中重复使用
        QString error1;
        QString error2;
        bool ok1 = kpService->validateSubjects(currentLeftIndex(), ui->kpCodeEdit->text().trimmed(), &error1);
        bool ok2 = kpService->validateSubjectKps(&error2);

        if (!ok1) { error1.prepend("学科:\n"); }
        if (!ok2) { error2.prepend("知识点:\n"); }

        if (!ok1 || !ok2) {
            QString error = error1 + "\n" + error2;
            error.replace("\n", "<br>");
            error = QString("<center><font color='red'>校验失败: 编码被重复使用</font></center><br>%1").arg(error);

            MessageBox::message(error, 400, 300);
        }

        return ok1 && ok2;
    }*/
}

// 左侧学科的树中当前选中节点的 index
QModelIndex KpEditor::currentLeftIndex() const {
    return UiUtil::getSelectedIndex(ui->subjectsTreeView);
}

// 右侧知识点树中当前选中节点的 index
QModelIndex KpEditor::currentKpIndex() const {
    return UiUtil::getSelectedIndex(ui->kpsTreeView);
}

// 调用 saveSubjects() and saveKps()
void KpEditor::save() {
    // 保存逻辑:
    // 1. 验证不通过不进行保存
    // 2. 没选择学科则只保存学科结构到 kps.json
    // 3. 选择了学科，则要保存学科结构以及它的知识点
    //    3.1 更新 subjectCode 到学科结构中选中的学科
    //    3.2 删除旧的知识点文件 ${oldSubJectCode}.json
    //    3.3 保存学科结构到 kps.json
    //    3.4 保存当前的学科到 ${subjectCode}.json

    // [1] 验证不通过不进行保存
    if (!validate()) { return; }

    if (Service::isSubjectIndex(currentLeftIndex())) {
        // [3] 选择了学科，则要保存学科结构以及它的知识点

        // 保存当前的学科到 ${subjectCode}.json
        bool ok1 = kpService->saveSubjectKps(subjectName, subjectCode);

        if (ok1) {
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    }

    /*if (Service::isSubjectIndex(currentLeftIndex())) {
        // [3] 选择了学科，则要保存学科结构以及它的知识点
        QString subjectName = ui->kpSubjectEdit->text().trimmed();
        QString subjectCode = ui->kpCodeEdit->text().trimmed();

        // [3.1] 更新 subjectCode 到学科结构中选中的学科，因为可能被编辑过了
        QString oldSubJectCode = currentLeftIndex().data(ROLE_CODE).toString();
        subjectsModel->setData(currentLeftIndex(), subjectCode, ROLE_CODE);

        // [3.2] 删除旧的知识点文件 ${oldSubJectCode}.json
        if (oldSubJectCode != subjectCode) {
            QFile::remove(Service::subjectKpsFilePath(kpsDir, oldSubJectCode));
        }

        // [3.3] 保存学科结构到 kps.json
        // [3.4] 保存当前的学科到 ${subjectCode}.json
        bool ok1 = kpService->saveSubjects();
        bool ok2 = kpService->saveSubjectKps(subjectName, subjectCode);

        if (ok1 && ok2) {
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    } else {
        // [2] 没选择学科则只保存学科结构到 kps.json
        bool ok = kpService->saveSubjects();

        if (ok) {
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    }*/
}
