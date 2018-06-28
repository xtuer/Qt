#include "ui_KpEditor.h"
#include "KpEditor.h"
#include "MessageBox.h"
#include "bean/Book.h"
#include "util/UiUtil.h"
#include "service/KpService.h"

#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>

KpEditor::KpEditor(QWidget *parent) : QWidget(parent), ui(new Ui::KpEditor) {
    initialize();
    handleEvents();

    openSubjects();
}

KpEditor::~KpEditor() {
    delete ui;
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

    // 创建 kps 文件夹
    kpsDir = QDir("data/kps");
    if (!kpsDir.exists()) {
        QDir("data").mkdir("kps");
    }

    // 左侧学科的树
    subjectsModel = new QStandardItemModel(this);
    subjectsModel->setHorizontalHeaderLabels(QStringList() << "学科 (阶段 > 学科)");
    ui->subjectsTreeView->setModel(subjectsModel);

    // 知识点的树
    kpsModel = new QStandardItemModel(this);
    kpsModel->setHorizontalHeaderLabels(QStringList() << "知识点" << "编码");
    ui->kpsTreeView->setModel(kpsModel);
    ui->kpsTreeView->setColumnWidth(0, 300);

    createSubjectsContextMenu(); // 创建左侧学科的右键菜单
    createKpsContextMenu();      // 创建中间知识点右键菜单

    kpService = new KpService(subjectsModel, kpsModel);
}

// 事件处理
void KpEditor::handleEvents() {
    // 点击教材树上的节点，如果是教材类型的话打开对应的教材
    connect(ui->subjectsTreeView, &QTreeView::clicked, [this] (const QModelIndex &index) {
        resetKps();
        currentLeftIndex = index;

        if (kpService->isSubjectIndex(index)) {
            QString subjectName = index.data().toString();
            QString subjectCode = index.data(ROLE_CODE).toString();
            ui->kpSubjectEdit->setText(subjectName);
            ui->kpCodeEdit->setText(subjectCode);

            openSubjectKps(subjectCode); // 打开学科的知识点
            // originalBookCode = bookCode; // 加载教材时保存教材的编码
        }
    });

    // 编辑学科树上学科节点的名字时，更新右边的学科名字
    connect(subjectsModel, &QStandardItemModel::itemChanged, [this] (QStandardItem *item) {
        if (kpService->isSubjectIndex(item->index())) {
            QString subjectName = item->data(Qt::DisplayRole).toString();
            ui->kpSubjectEdit->setText(subjectName);
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
}

// 创建左侧学科的右键菜单
void KpEditor::createSubjectsContextMenu() {
    // 左侧教材树的右键菜单: 阶段 > 学科 > 教材
    QAction *phaseAction    = new QAction("新建阶段", this);
    QAction *subjectAction  = new QAction("新建学科", this);
    QAction *deleteAction   = new QAction("删除操作", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->subjectsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧学科树节点，弹出右键菜单
    connect(ui->subjectsTreeView, &QWidget::customContextMenuRequested, [=]() {
        // 根据 index 类型判断要显示的右键菜单项
        // 1. 点击阶段显示新建学科
        // 2. 点击空白显示新建阶段

        QMenu menu;
        QModelIndex index = UiUtil::indexAt(ui->subjectsTreeView, QCursor::pos());
        currentLeftIndex = index;

        if (kpService->isPhaseIndex(index)) {
            menu.addAction(subjectAction); // [1] 显示新建学科
            menu.addSeparator();
            deleteAction->setText("删除阶段");
            menu.addAction(deleteAction);
        } else if (kpService->isSubjectIndex(index)) {
            deleteAction->setText("删除学科");
            menu.addAction(deleteAction);
        } else {
            menu.addAction(phaseAction); // [2] 显示新建阶段
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 创建新的阶段
    connect(phaseAction, &QAction::triggered, [this] {
        QStandardItem *phaseItem = new QStandardItem("新建阶段");
        phaseItem->setData(TYPE_PHASE, ROLE_TYPE); // 表示学科
        subjectsModel->appendRow(phaseItem);
    });

    // 创建新的学科
    connect(subjectAction, &QAction::triggered, [this] {
        if (!kpService->isPhaseIndex(currentLeftIndex)) { return; }

        QStandardItem *phaseItem = subjectsModel->itemFromIndex(currentLeftIndex);
        QStandardItem *subjectItem = new QStandardItem("新建学科");
        subjectItem->setData(TYPE_SUBJECT, ROLE_TYPE); // 表示学科
        phaseItem->appendRow(subjectItem);

        ui->subjectsTreeView->expand(currentLeftIndex);
    });

    // 删除操作
    connect(deleteAction, &QAction::triggered, [this] {
        if (!currentLeftIndex.isValid()) { return; }

        QString name = currentLeftIndex.data().toString();
        int rowCount = subjectsModel->itemFromIndex(currentLeftIndex)->rowCount();

        if (kpService->isPhaseIndex(currentLeftIndex)) {
            // 删除阶段
            // 阶段下还有学科时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有学科，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除阶段 <font color='darkred'>%1</font> 吗?").arg(name))) {
                subjectsModel->removeRow(currentLeftIndex.row(), currentLeftIndex.parent());
            }
        } else if (kpService->isSubjectIndex(currentLeftIndex)) {
            // 删除学科
            if (MessageBox::confirm(QString("确定要删除学科 <font color='darkred'>%1</font> 吗?").arg(name))) {
                subjectsModel->removeRow(currentLeftIndex.row(), currentLeftIndex.parent());
                resetKps();
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
    QAction *createAction   = new QAction("创建知识点", this);
    QAction *deleteAction   = new QAction("删除知识点", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->kpsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧学科树节点，弹出右键菜单
    connect(ui->kpsTreeView, &QWidget::customContextMenuRequested, [=]() {
        if (!kpService->isSubjectIndex(currentLeftIndex)) {
            return;
        }

        QMenu menu;
        QModelIndex index = UiUtil::indexAt(ui->kpsTreeView, QCursor::pos());
        currentKpIndex = index;

        menu.addAction(createAction);

        if (index.isValid()) {
            menu.addAction(deleteAction);
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 创建新的知识点
    connect(createAction, &QAction::triggered, [this] {
        if (!kpService->isSubjectIndex(currentLeftIndex)) {
            MessageBox::message("请先选中学科，然后再创建知识点");
            return;
        }

        // 新知识点的 code 为: (parent 的 code) + (parent 的知识点数+1)
        QList<QStandardItem*> childColumns;

        if (currentKpIndex.isValid()) {
            // 右键点击知识点名字或者编码创建子知识点
            QModelIndex nameIndex = kpsModel->index(currentKpIndex.row(), 0, currentKpIndex.parent());
            QModelIndex codeIndex = kpsModel->index(currentKpIndex.row(), 1, currentKpIndex.parent());

            int rowCount = kpsModel->rowCount(nameIndex) + 1;
            QString parentCode = codeIndex.data().toString();
            QString childCode = QString("%1%2").arg(parentCode).arg(rowCount, 2, 10, QChar('0'));

            childColumns << new QStandardItem("新建知识点") << new QStandardItem(childCode);
            kpsModel->itemFromIndex(nameIndex)->appendRow(childColumns);
            ui->kpsTreeView->expand(nameIndex);
        } else {
            // 右键点击空白处创建第一级的知识点
            int rowCount = kpsModel->rowCount() + 1;
            QString childCode = QString("%1").arg(rowCount, 2, 10, QChar('0'));

            childColumns << new QStandardItem("新建知识点") << new QStandardItem(childCode);
            kpsModel->appendRow(childColumns);
        }
    });
}

// 重置知识点
void KpEditor::resetKps() {
    ui->kpSubjectEdit->setText("");
    ui->kpCodeEdit->setText("");
    kpsModel->removeRows(0, kpsModel->rowCount());
}

// 打开学科到左侧的教材目录树中
void KpEditor::openSubjects() {
    resetKps();

    // 打开 kps/kps.json
    QFileInfo info = QFileInfo(kpsDir.filePath("kps.json"));

    if (info.exists()) {
        kpService->readSubjects(info.absoluteFilePath());
        ui->subjectsTreeView->expandAll();
        UiUtil::showMessage(ui->messageLabel, "打开成功");
    }
}

// 打开学科的知识点到右侧的知识点树中
void KpEditor::openSubjectKps(const QString &subjectCode) {
    QFileInfo kpsFileInfo(kpsDir, subjectCode + ".json");

    if (kpsFileInfo.exists()) {
        kpService->readSubjectKps(kpsFileInfo.absoluteFilePath());
        ui->kpsTreeView->expandAll();
    } else if (!subjectCode.isEmpty()) {
        MessageBox::message(QString("文件 kps/%1.json 不存在").arg(subjectCode));
    }
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

    if (!kpService->isSubjectIndex(currentLeftIndex)) {
        // [1] 没选择学科则只校验学科结构
        QString error;
        bool ok = kpService->validateSubjects(currentLeftIndex, ui->kpCodeEdit->text().trimmed(), &error);

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
        bool ok1 = kpService->validateSubjects(currentLeftIndex, ui->kpCodeEdit->text().trimmed(), &error1);
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
    }
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

    if (kpService->isSubjectIndex(currentLeftIndex)) {
        // [3] 选择了学科，则要保存学科结构以及它的知识点
        QString subjectName = ui->kpSubjectEdit->text().trimmed();
        QString subjectCode = ui->kpCodeEdit->text().trimmed();

        // [3.1] 更新 subjectCode 到学科结构中选中的学科，因为可能被编辑过了
        QString oldSubJectCode = currentLeftIndex.data(ROLE_CODE).toString();
        subjectsModel->setData(currentLeftIndex, subjectCode, ROLE_CODE);

        // [3.2] 删除旧的知识点文件 ${oldSubJectCode}.json
        if (oldSubJectCode != subjectCode) {
            QFile::remove(kpsDir.filePath(oldSubJectCode + ".json"));
        }

        // [3.3] 保存学科结构到 kps.json
        // [3.4] 保存当前的学科到 ${subjectCode}.json
        bool ok1 = kpService->saveSubjects(kpsDir);
        bool ok2 = kpService->saveSubjectKps(subjectName, subjectCode, kpsDir);

        if (ok1 && ok2) {
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    } else {
        // [2] 没选择学科则只保存学科结构到 kps.json
        bool ok = kpService->saveSubjects(kpsDir);

        if (ok) {
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    }
}
