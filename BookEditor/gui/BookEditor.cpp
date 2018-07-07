#include "ui_BookEditor.h"
#include "BookEditor.h"
#include "KpEditor.h"
#include "TopWindow.h"
#include "MessageBox.h"

#include "bean/Book.h"
#include "model/BooksModel.h"
#include "model/ChaptersModel.h"

#include "util/Json.h"
#include "util/UiUtil.h"
#include "util/Config.h"
#include "service/Service.h"
#include "service/BookService.h"

#include <QDebug>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QList>
#include <algorithm>
#include <QRegularExpressionValidator>
#include <QTimer>
#include <QVBoxLayout>

BookEditor::BookEditor(QWidget *parent) : QWidget(parent), ui(new Ui::BookEditor) {
    initialize();
    handleEvents();

    openBooks();
}

BookEditor::~BookEditor() {
    delete ui;
    delete bookService;
}

bool BookEditor::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->messageLabel && event->type() == QEvent::Show) {
        QTimer::singleShot(2000, ui->messageLabel, SLOT(hide()));
    }

    return QWidget::eventFilter(watched, event);
}

// 初始化 Ui
void BookEditor::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->messageLabel->hide();
    ui->messageLabel->installEventFilter(this);

    // 创建 books 文件夹
    booksDir = QDir("data/books");
    if (!booksDir.exists()) {
        booksDir.mkpath(".");
    }

    // 左侧教材的树
    booksModel = new BooksModel(this);
    booksModel->setHorizontalHeaderLabels(QStringList() << "学段 > 学科 > 版本 > 教材");
    ui->booksTreeView->setModel(booksModel);

    // 教材章节目录的树
    chaptersModel = new ChaptersModel(this);
    chaptersModel->setHorizontalHeaderLabels(QStringList() << "章节" << "编码");
    ui->chaptersTreeView->setModel(chaptersModel);
    ui->chaptersTreeView->hideColumn(1); // 隐藏编码列
    ui->chaptersTreeView->setColumnWidth(0, 300);

    // 启用拖拽
    // ui->booksTreeView->setDragEnabled(true);
    // ui->booksTreeView->setAcceptDrops(true);
    // ui->booksTreeView->setDropIndicatorShown(true);
    // ui->booksTreeView->setDragDropMode(QAbstractItemView::InternalMove);
    // ui->booksTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // ui->chaptersTreeView->setDragEnabled(true);
    // ui->chaptersTreeView->setAcceptDrops(true);
    // ui->chaptersTreeView->setDropIndicatorShown(true);
    // ui->chaptersTreeView->setDragDropMode(QAbstractItemView::InternalMove);
    // ui->chaptersTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    previewButton = UiUtil::createLineEditRightButton(ui->bookCoverEdit); // 创建封面预览按钮
    previewButton->setObjectName("previewButton");

    // 设置编码的 validator，只运输字母、数字和下划线
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[-\\w]+"), this);
    ui->bookCodeEdit->setValidator(validator);

    createBooksContextMenu();    // 创建左侧教材树的右键菜单
    createChaptersContextMenu(); // 创建中间章节树的右键菜单

    bookService = new BookService(booksModel, chaptersModel);
}

// 事件处理
void BookEditor::handleEvents() {
    // 教材树上的当前节点变化时，如果是教材类型的话打开对应的教材
    connect(ui->booksTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this](const QModelIndex &index) {
        resetBook();

        if (Service::isBookIndex(index)) {
            QString subjectName = index.parent().parent().data().toString();
            QString versionName = index.parent().data().toString();
            QString bookName    = index.data().toString();
            QString bookCode    = index.data(ROLE_CODE).toString();
            QString bookCover   = index.data(ROLE_COVER).toString();

            ui->bookSubjectEdit->setText(subjectName);
            ui->bookVersionEdit->setText(versionName);
            ui->bookCodeEdit->setText(bookCode);
            ui->bookNameEdit->setText(bookName);
            ui->bookCoverEdit->setText(bookCover);

            openBook(bookCode); // 打开教材
        }
    });

    // 编辑教材树上教材节点的名字时，更新右边的教材名字
    connect(booksModel, &QStandardItemModel::itemChanged, [this] (QStandardItem *item) {
        if (Service::isBookIndex(item->index())) {
            QString bookName = item->data(Qt::DisplayRole).toString();
            ui->bookNameEdit->setText(bookName);
        }
    });

    // 教材、章节节点的内容变化后自动保存
    connect(booksModel, &QStandardItemModel::itemChanged, [this] () {
        ui->saveButton->click();
    });
    connect(chaptersModel, &QStandardItemModel::itemChanged, [this] () {
        ui->saveButton->click();
    });

    // // 编辑教材名字时，更新到树当前选择的教材节点
    // connect(ui->bookVersionEdit, &QLineEdit::editingFinished, [this] {
    //     if (bookService->isBookIndex(currentBookIndex)) {
    //         QString bookVersion = ui->bookVersionEdit->text().trimmed();
    //         booksModel->setData(currentBookIndex, bookVersion);
    //     }
    // });

    // // 编辑教材编码时，更新到树当前选择的教材节点
    // connect(ui->bookCodeEdit, &QLineEdit::editingFinished, [this] {
    //     if (bookService->isBookIndex(currentBookIndex)) {
    //         QString bookCode = ui->bookCodeEdit->text().trimmed();
    //         booksModel->setData(currentBookIndex, bookCode, ROLE_CODE);
    //     }
    // });

    // 预览封面
    connect(previewButton, &QPushButton::clicked, [this] {
        QString coverUrl = ui->bookCoverEdit->text().trimmed();

        if (!coverUrl.isEmpty()) {
            UiUtil::previewImage(coverUrl, "temp");
        }
    });

    // 校验
    connect(ui->checkButton, &QPushButton::clicked, [this] {
        if (validate()) {
            UiUtil::showMessage(ui->messageLabel, "验证通过");
        }
    });

    // 点击打开按钮，加载所有的教材信息
    connect(ui->reopenButton, &QPushButton::clicked, [this] {
        openBooks();
    });

    // 保存
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        save();
    });
}

// 创建左侧教材树的右键菜单
void BookEditor::createBooksContextMenu() {
    // 左侧教材树的右键菜单: 阶段 > 学科 > 教材
    QAction *phaseAction    = new QAction("新建阶段", this);
    QAction *subjectAction  = new QAction("新建学科", this);
    QAction *versionAction  = new QAction("新建版本", this);
    QAction *bookAction     = new QAction("新建教材", this);
    QAction *deleteAction   = new QAction("删除操作", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->booksTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧教材树节点，弹出右键菜单
    connect(ui->booksTreeView, &QWidget::customContextMenuRequested, [=]() {
        // 根据 index 类型判断要显示的右键菜单项
        // 1. 点击阶段显示新建学科
        // 2. 点击学科显示新建教材
        // 3. 点击教材什么都不显示
        // 4. 点击空白显示新建阶段

        QMenu menu;
        QModelIndex index = UiUtil::indexAt(ui->booksTreeView, QCursor::pos());
        leftClickedLeftIndex = index;

        if (Service::isPhaseIndex(index)) {
            deleteAction->setText("删除阶段");
            menu.addAction(subjectAction); // [1] 显示新建学科
            menu.addAction(deleteAction);
        } else if (Service::isSubjectIndex(index)) {
            deleteAction->setText("删除学科");
            menu.addAction(versionAction); // [2] 显示新建版本
            menu.addAction(deleteAction);
        } else if (Service::isVersionIndex(index)) {
            deleteAction->setText("删除版本");
            menu.addAction(bookAction); // [2] 显示新建教材
            menu.addAction(deleteAction);
        } else if (Service::isBookIndex(index)) {
            deleteAction->setText("删除教材");
            menu.addAction(deleteAction);
        } else {
            menu.addAction(phaseAction); // [4] 显示新建阶段
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 创建新的阶段
    connect(phaseAction, &QAction::triggered, [this] {
        Service::appendRow(booksModel, QModelIndex(), { Service::createPhaseItem("新建阶段") });
    });

    // 创建新的学科
    connect(subjectAction, &QAction::triggered, [this] {
        if (!Service::isPhaseIndex(leftClickedLeftIndex)) { return; }

        Service::appendRow(booksModel, leftClickedLeftIndex, { Service::createSubjectItem("新建学科") });
        ui->booksTreeView->expand(leftClickedLeftIndex);
    });

    // 创建新的版本
    connect(versionAction, &QAction::triggered, [this] {
        if (!Service::isSubjectIndex(leftClickedLeftIndex)) { return; }

        Service::appendRow(booksModel, leftClickedLeftIndex, { Service::createVersionItem("新建版本") });
        ui->booksTreeView->expand(leftClickedLeftIndex);
    });

    // 创建新的教材
    connect(bookAction, &QAction::triggered, [this] {
        if (!Service::isVersionIndex(leftClickedLeftIndex)) { return; }

        Service::appendRow(booksModel, leftClickedLeftIndex, { Service::createBookItem("新建教材", "", "") });
        ui->booksTreeView->expand(leftClickedLeftIndex);
    });

    // 删除操作
    connect(deleteAction, &QAction::triggered, [this] {
        if (!leftClickedLeftIndex.isValid()) { return; }

        QModelIndex current = leftClickedLeftIndex;
        QString name = current.data().toString();
        int rowCount = booksModel->itemFromIndex(current)->rowCount();

        if (Service::isPhaseIndex(current)) {
            // 删除阶段
            // 阶段下还有学科时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有学科，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除阶段 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(current.row(), current.parent());
            }
        } else if (Service::isSubjectIndex(current)) {
            // 删除学科
            // 学科下还有教材时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有版本，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除学科 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(current.row(), current.parent());
            }
        } else if (Service::isVersionIndex(current)) {
            // 删除版本
            // 版本下还有教材时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有教材，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除版本 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(current.row(), current.parent());
            }
        } else if (Service::isBookIndex(current)) {
            // 删除教材
            if (MessageBox::confirm(QString("确定要删除版本 <font color='darkred'>%1</font> 吗?").arg(name))) {
                // 删除此教材的章节文件
                QString bookCode = current.data(ROLE_CODE).toString().trimmed();
                QFile::remove(Service::chapterFilePath(booksDir, bookCode));

                // 从树中删除学教材点
                booksModel->removeRow(current.row(), current.parent());

                // 删除的时候保存一下
                ui->saveButton->click();
            }
        }
    });

    // 全部展开教材树
    connect(expandAction, &QAction::triggered, [this] {
        ui->booksTreeView->expandAll();
    });

    // 全部收拢教材树
    connect(collapseAction, &QAction::triggered, [this] {
        ui->booksTreeView->collapseAll();
    });
}

// 创建中间章节树的右键菜单
void BookEditor::createChaptersContextMenu() {
    QAction *appendChildChapterAction = new QAction("添加子章节", this);
    QAction *appendKpAction           = new QAction("添加知识点", this);
    QAction *insertBeforeAction = new QAction("插入前一章节", this);
    QAction *insertAfterAction  = new QAction("插入后一章节", this);
    QAction *deleteAction   = new QAction("删除", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->chaptersTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击右侧章节的树节点，弹出右键菜单
    connect(ui->chaptersTreeView, &QWidget::customContextMenuRequested, [=]() {
        // 如果左侧没有选中教材，则不显示右键菜单
        if (!Service::isBookIndex(currentLeftIndex())) {
            return;
        }

        QMenu menu;
        rightClickedChapterIndex = UiUtil::indexAt(ui->chaptersTreeView, QCursor::pos()); // 右键点击处的 index

        if (Service::isKpIndex(rightClickedChapterIndex)) {
            // 知识点的节点只能删除
            menu.addAction(deleteAction);
        } else {
            // 右键点击其他有效的 index，则显示: 插入前一章节、插入后一章节、添加知识点、删除
            menu.addAction(appendChildChapterAction);

            if (rightClickedChapterIndex.isValid()) {
                menu.addSeparator();
                menu.addAction(insertBeforeAction);
                menu.addAction(insertAfterAction);
                menu.addSeparator();
                menu.addAction(appendKpAction);
                menu.addSeparator();
                menu.addAction(deleteAction);
            }

            menu.addSeparator();
            menu.addAction(expandAction);
            menu.addAction(collapseAction);
        }

        menu.exec(QCursor::pos());
    });

    // 创建子章节
    connect(appendChildChapterAction, &QAction::triggered, [this] {
        bookService->appendChildChapter(rightClickedChapterIndex);
        ui->chaptersTreeView->expand(rightClickedChapterIndex);
    });

    // 插入前一章节
    connect(insertBeforeAction, &QAction::triggered, [this] {
        bookService->insertChapter(rightClickedChapterIndex, true);
    });

    // 插入后一章节
    connect(insertAfterAction, &QAction::triggered, [this] {
        bookService->insertChapter(rightClickedChapterIndex, false);
    });

    // 添加知识点
    connect(appendKpAction, &QAction::triggered, [this] {
        // 把 kpEditor 放在一个 widget 中，设置 widget 的背景色为白色
        KpEditor *kpEditor = new KpEditor(true);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(kpEditor);

        QWidget *centralWidget = new QWidget();
        centralWidget->setObjectName("kpEditorHolder");
        centralWidget->setStyleSheet("#kpEditorHolder { background: white; }");
        centralWidget->setLayout(layout);
        UiUtil::setWidgetPaddingAndSpacing(centralWidget, 0, 0);

        // 显示模态弹出窗口
        TopWindow dialog(centralWidget);
        dialog.setTitleBarButtonsVisible(false, false, true);
        dialog.resize(800, 600);
        dialog.showModal();

        // 点击 KpEditor 的确定按钮时获取选中的知识点
        if (kpEditor->isOkButtonClickedInReadOnlyMode()) {
            QStringList kp = kpEditor->getSelectedKp();

            if (!kp.isEmpty()) {
                QString name = kp.value(0);
                QString code = kp.value(1);
                QString subjectCode = kp.value(2);
                bookService->appendKpOfChapter(rightClickedChapterIndex, name, code, subjectCode);
            }
        }
    });

    // 删除章节
    connect(deleteAction, &QAction::triggered, [this] {
        if (!rightClickedChapterIndex.isValid()) { return; }

        QModelIndex nameIndex = rightClickedChapterIndex.sibling(rightClickedChapterIndex.row(), 0);
        QString name = nameIndex.data().toString();

        // 如果有子章节，不让删除，避免误删除
        if (chaptersModel->rowCount(nameIndex) > 0) {
            MessageBox::message(QString("<font color='darkred'>%1</font> 下还有章节，不能删除").arg(name));
        } else if (MessageBox::confirm(QString("确定要删除 <font color='darkred'>%1</font> 吗?").arg(name))) {
            chaptersModel->removeRow(nameIndex.row(), nameIndex.parent());
        }
    });

    // 全部展开章节目录树
    connect(expandAction, &QAction::triggered, [this] {
        ui->chaptersTreeView->expandAll();
    });

    // 全部收拢章节目录树
    connect(collapseAction, &QAction::triggered, [this] {
        ui->chaptersTreeView->collapseAll();
    });
}

// 打开教材内容显示到右边
void BookEditor::openBook(const QString &bookCode) {
    QFileInfo chapterFileInfo(Service::chapterFilePath(booksDir, bookCode)); // 教材文件信息

    if (chapterFileInfo.exists()) {
        bookService->readChapters(chapterFileInfo.absoluteFilePath());
        ui->chaptersTreeView->expandAll();
    } else if (!bookCode.isEmpty()) {
        // 文件不存在且教材编码为空
        MessageBox::message(QString("文件 books/%1 不存在").arg(chapterFileInfo.fileName()));
    }
}

// 当前教材的 index
QModelIndex BookEditor::currentLeftIndex() const {
    return UiUtil::getSelectedIndex(ui->booksTreeView);
}

// 当前章节的 index
QModelIndex BookEditor::currentChapterIndex() const {
    return UiUtil::getSelectedIndex(ui->chaptersTreeView);
}

// 打开教材结构显示到左侧的教材目录树中
void BookEditor::openBooks() {
    resetBook();

    // 打开 kps/kps.json
    QFileInfo info = QFileInfo(booksDir.filePath("books.json"));

    if (info.exists()) {
        bookService->readBooks(info.absoluteFilePath());
        ui->booksTreeView->expandAll();
        UiUtil::showMessage(ui->messageLabel, "打开成功");
    }
}

// 重置右边的教材信息
void BookEditor::resetBook() {
    ui->bookCodeEdit->setText("");
    ui->bookSubjectEdit->setText("");
    ui->bookCoverEdit->setText("");
    ui->bookVersionEdit->setText("");
    ui->bookNameEdit->setText("");
    chaptersModel->removeRows(0, chaptersModel->rowCount());
}

// 校验教材和章节的编码是否唯一未被重复使用
bool BookEditor::validate() const {
    // 校验逻辑:
    // 1. 没选择教材则只校验教材结构
    // 2. 选择教材不但要校验教材结构，还要校验教材内容
    //    2.1 教材的学科不能为空
    //    2.2 教材的版本不能为空
    //    2.3 教材的编码为空且章节为空，则返回 true
    //    2.4 教材的编码为空且章节不为空，则返回 false
    //    2.5 教材编码不能重复使用
    //    2.6 章节编码不能在当前教材中重复使用

    if (!Service::isBookIndex(currentLeftIndex())) {
        // [1] 没选择教材则只校验教材结构
        QString error;
        bool ok = bookService->validateBooks(currentLeftIndex(), ui->bookCodeEdit->text().trimmed(), &error);

        if (!ok) {
            error.replace("\n", "<br>");
            error = QString("<center><font color='red'>校验失败:</font></center><br>%1").arg(error);

            MessageBox::message(error, 400, 300);
        }

        return ok;
    } else {
        // [2] 选择教材不但要校验教材结构，还要校验教材内容
        // [2.1] 教材的学科不能为空
        if (ui->bookSubjectEdit->text().trimmed().isEmpty()) {
            MessageBox::message("教材的 <font color='red'>学科</font> 不能为空");
            return false;
        }

        // [2.2] 教材的版本不能为空
        if (ui->bookVersionEdit->text().trimmed().isEmpty()) {
            MessageBox::message("教材的 <font color='red'>版本</font> 不能为空");
            return false;
        }

        // [2.3] 教材的编码为空且章节为空，则返回 true
        if (ui->bookCodeEdit->text().trimmed().isEmpty() && chaptersModel->rowCount() == 0) {
            return true;
        }

        // 2.4 教材的编码为空且章节不为空，则返回 false
        if (ui->bookCodeEdit->text().trimmed().isEmpty() && chaptersModel->rowCount() > 0) {
            MessageBox::message("教材的 <font color='red'>编码</font> 不能为空");
            return false;
        }

        // [2.5] 教材编码不能重复使用
        // [2.6] 章节编码不能在当前教材中重复使用
        QString error1;
        QString error2;
        bool ok1 = bookService->validateBooks(currentLeftIndex(), ui->bookCodeEdit->text().trimmed(), &error1);
        bool ok2 = bookService->validateChapters(&error2);

        if (!ok1) { error1.prepend("教材:\n"); }
        if (!ok2) { error2.prepend("章节:\n"); }

        if (!ok1 || !ok2) {
            QString error = error1 + "\n" + error2;
            error.replace("\n", "<br>");
            error = QString("<center><font color='red'>校验失败: 编码被重复使用</font></center><br>%1").arg(error);

            MessageBox::message(error, 400, 300);
        }

        return ok1 && ok2;
    }
}

void BookEditor::save() {
    // 保存逻辑:
    // 1. 验证不通过不进行保存
    // 2. 没选择教材则只保存教材结构到 books.json
    // 3. 选择教材不但要保存教材结构，还要保存教材内容
    //    3.1 更新 bookCode 到教材结构中选中的教材，originalBookCode 赋值为 bookCode，用于下次更新删除无效教材文件
    //    3.2 删除旧的教材文件 ${originalBookCode}.json
    //    3.3 保存教材结构到 books.json
    //    3.4 保存当前的教材到 ${bookCode}.json

    // [1] 验证不通过不进行保存
    if (!validate()) { return; }

    if (!Service::isBookIndex(currentLeftIndex())) {
        // [2] 没选择教材则只保存教材结构到 books.json
        bool ok = bookService->saveBooks(booksDir);

        if (ok) {
            // MessageBox::message("<center><font color='green'>保存成功</fong></center>");
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            // MessageBox::message("<center><font color='red'>保存失败</fong></center>");
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    } else {
        // [3] 选择教材不但要保存教材结构，还要保存教材内容
        QString bookSubject = ui->bookSubjectEdit->text().trimmed();
        QString bookVersion = ui->bookVersionEdit->text().trimmed();
        QString bookName    = ui->bookNameEdit->text().trimmed();
        QString bookCode    = ui->bookCodeEdit->text().trimmed();
        QString bookCover   = ui->bookCoverEdit->text().trimmed();

        // [3.1] 更新 bookCode, bookCover 到教材结构中选中的教材
        QString oldBookCode = currentLeftIndex().data(ROLE_CODE).toString();
        booksModel->setData(currentLeftIndex(), bookCode, ROLE_CODE);
        booksModel->setData(currentLeftIndex(), bookCover, ROLE_COVER);

        // [3.2] 删除旧的教材文件 ${oldBookCode}.json
        if (oldBookCode != bookCode) {
            QFile::remove(Service::chapterFilePath(booksDir, oldBookCode));
        }

        // [3.3] 保存教材结构到 books.json
        // [3.4] 保存当前的教材到 ${bookCode}.json
        bool ok1 = bookService->saveBooks(booksDir);
        bool ok2 = bookService->saveChapters(bookCode, bookSubject, bookVersion, bookName, bookCover, booksDir);

        if (ok1 && ok2) {
            // MessageBox::message("<center><font color='green'>保存成功</fong></center>");
            UiUtil::showMessage(ui->messageLabel, "保存成功");
        } else {
            // MessageBox::message("<center><font color='red'>保存失败</fong></center>");
            UiUtil::showMessage(ui->messageLabel, "保存失败", false);
        }
    }
}
