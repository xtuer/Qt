#include "ui_BookEditor.h"
#include "BookEditor.h"
#include "MessageBox.h"

#include "bean/Book.h"
#include "model/BooksModel.h"
#include "model/ChaptersModel.h"

#include "util/Json.h"
#include "util/UiUtil.h"
#include "util/Config.h"
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

BookEditor::BookEditor(QWidget *parent) : QWidget(parent), ui(new Ui::BookEditor) {
    initialize();
    handleEvents();

    // 打开上次保存的教材，如果不存在，则尝试打开 books/books.json
    booksDir = ConfigInstance.getBooksDir();
    QFileInfo info = QFileInfo(booksDir.filePath("books.json"));

    if (info.exists()) {
        openBooks(info.absoluteFilePath());
    } else {
        booksDir = QDir("books");
        openBooks("books/books.json");
    }
}

BookEditor::~BookEditor() {
    delete ui;
    delete bookService;
}

// 初始化 Ui
void BookEditor::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    // 左侧教材的树
    booksModel = new BooksModel(this);
    booksModel->setHorizontalHeaderLabels(QStringList() << "教材 (阶段 > 学科 > 版本)");
    ui->booksTreeView->setModel(booksModel);

    // 教材章节目录的树
    chaptersModel = new ChaptersModel(this);
    chaptersModel->setHorizontalHeaderLabels(QStringList() << "章节" << "编码");
    ui->chaptersTreeView->setModel(chaptersModel);
    ui->chaptersTreeView->setColumnWidth(0, 250);

    // 启用拖拽
    ui->booksTreeView->setDragEnabled(true);
    ui->booksTreeView->setAcceptDrops(true);
    ui->booksTreeView->setDropIndicatorShown(true);
    ui->booksTreeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->booksTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->chaptersTreeView->setDragEnabled(true);
    ui->chaptersTreeView->setAcceptDrops(true);
    ui->chaptersTreeView->setDropIndicatorShown(true);
    ui->chaptersTreeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->chaptersTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

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
    // 点击打开按钮，加载所有的教材信息
    connect(ui->openButton, &QPushButton::clicked, [this] {
        QString filename = QFileDialog::getOpenFileName(this, "打开教材", ".", "教材 (*.json)");

        if (!filename.isEmpty()) {
            openBooks(filename);
            booksDir = QFileInfo(filename).dir();
            ConfigInstance.setBooksDir(booksDir.canonicalPath());
        }
    });

    // 点击教材树上的节点，如果是教材类型的话打开对应的教材
    connect(ui->booksTreeView, &QTreeView::clicked, [this] (const QModelIndex &index) {
        resetBook();
        currentBookIndex = index;

        if (bookService->isBookIndex(index)) {
            QString subjectName = index.parent().data().toString();
            QString versionName = index.data().toString();
            QString bookCode    = index.data(ROLE_CODE).toString();
            ui->bookSubjectEdit->setText(subjectName);
            ui->bookVersionEdit->setText(versionName);
            ui->bookCodeEdit->setText(bookCode);

            QFileInfo bookFileInfo(booksDir, bookCode + ".json"); // 教材文件信息
            openBook(bookFileInfo.absoluteFilePath()); // 打开教材
            originalBookCode = bookCode; // 加载教材时保存教材的编码
        }
    });

    // 编辑教材树上教材节点的名字时，更新右边的教材名字
    connect(booksModel, &QStandardItemModel::itemChanged, [this] (QStandardItem *item) {
        if (bookService->isBookIndex(item->index())) {
            QString bookVersion = item->data(Qt::DisplayRole).toString();
            ui->bookVersionEdit->setText(bookVersion);
        }
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
            MessageBox::message("<center><font color='green'>验证通过</font></center>");
        }
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
        currentBookIndex = index;

        if (bookService->isPhaseIndex(index)) {
            deleteAction->setText("删除阶段");
            menu.addAction(subjectAction); // [1] 显示新建学科
            menu.addAction(deleteAction);
        } else if (bookService->isSubjectIndex(index)) {
            deleteAction->setText("删除学科");
            menu.addAction(bookAction); // [2] 显示新建教材
            menu.addAction(deleteAction);
        } else if (bookService->isBookIndex(index)) {
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
        QStandardItem *phaseItem = new QStandardItem("新建阶段");
        phaseItem->setData(TYPE_PHASE, ROLE_TYPE); // 表示学科
        booksModel->appendRow(phaseItem);
    });

    // 创建新的学科
    connect(subjectAction, &QAction::triggered, [this] {
        if (!bookService->isPhaseIndex(currentBookIndex)) { return; }

        QStandardItem *phaseItem = booksModel->itemFromIndex(currentBookIndex);
        QStandardItem *subjectItem = new QStandardItem("新建学科");
        subjectItem->setData(TYPE_SUBJECT, ROLE_TYPE); // 表示学科
        phaseItem->appendRow(subjectItem);

        ui->booksTreeView->expand(currentBookIndex);
    });

    // 创建新的教材
    connect(bookAction, &QAction::triggered, [this] {
        if (!bookService->isSubjectIndex(currentBookIndex)) { return; }

        QStandardItem *subjectItem = booksModel->itemFromIndex(currentBookIndex);
        QStandardItem *bookItem = new QStandardItem("新建教材");
        bookItem->setData(TYPE_BOOK, ROLE_TYPE); // 表示教材
        subjectItem->appendRow(bookItem);

        ui->booksTreeView->expand(currentBookIndex);
    });

    // 删除操作
    connect(deleteAction, &QAction::triggered, [this] {
        if (!currentBookIndex.isValid()) { return; }

        QString name = currentBookIndex.data().toString();
        int rowCount = booksModel->itemFromIndex(currentBookIndex)->rowCount();

        if (bookService->isPhaseIndex(currentBookIndex)) {
            // 删除阶段
            // 阶段下还有学科时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有学科，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除阶段 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(currentBookIndex.row(), currentBookIndex.parent());
            }
        } else if (bookService->isSubjectIndex(currentBookIndex)) {
            // 删除学科
            // 学科下还有教材时不能被删除
            if (rowCount > 0) {
                MessageBox::message(QString("<font color='darkred'>%1</font> 下还有教材，不能删除").arg(name));
                return;
            }

            if (MessageBox::confirm(QString("确定要删除学科 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(currentBookIndex.row(), currentBookIndex.parent());
            }
        } else if (bookService->isBookIndex(currentBookIndex)) {
            // 删除教材
            if (MessageBox::confirm(QString("确定要删除版本 <font color='darkred'>%1</font> 吗?").arg(name))) {
                booksModel->removeRow(currentBookIndex.row(), currentBookIndex.parent());
                resetBook();
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
    QAction *createAction   = new QAction("创建章节", this);
    QAction *deleteAction   = new QAction("删除章节", this);
    QAction *expandAction   = new QAction("全部展开", this);
    QAction *collapseAction = new QAction("全部收拢", this);

    ui->chaptersTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 右键点击左侧教材树节点，弹出右键菜单
    connect(ui->chaptersTreeView, &QWidget::customContextMenuRequested, [=]() {
        QMenu menu;
        QModelIndex index = UiUtil::indexAt(ui->chaptersTreeView, QCursor::pos());
        currentChapterIndex = index;

        menu.addAction(createAction);

        if (index.isValid()) {
            menu.addAction(deleteAction);
        }

        menu.addSeparator();
        menu.addAction(expandAction);
        menu.addAction(collapseAction);

        menu.exec(QCursor::pos());
    });

    // 创建新的章节
    connect(createAction, &QAction::triggered, [this] {
        if (!currentBookIndex.isValid()) {
            MessageBox::message("请先选中教材，然后再创建章节");
            return;
        }

        // 新章节的 code 为: (parent 的 code) + (parent 的章节数+1)
        QList<QStandardItem*> childColumns;

        if (currentChapterIndex.isValid()) {
            // 右键点击章节名字或者编码创建子章节
            QModelIndex nameIndex = chaptersModel->index(currentChapterIndex.row(), 0, currentChapterIndex.parent());
            QModelIndex codeIndex = chaptersModel->index(currentChapterIndex.row(), 1, currentChapterIndex.parent());

            int rowCount = chaptersModel->rowCount(nameIndex) + 1;
            QString parentCode = codeIndex.data().toString();
            QString childCode = QString("%1%2").arg(parentCode).arg(rowCount, 2, 10, QChar('0'));

            childColumns << new QStandardItem("新建章节") << new QStandardItem(childCode);
            chaptersModel->itemFromIndex(nameIndex)->appendRow(childColumns);
            ui->chaptersTreeView->expand(nameIndex);
        } else {
            // 右键点击空白处创建第一级的章节
            int rowCount = chaptersModel->rowCount() + 1;
            QString childCode = QString("%1").arg(rowCount, 2, 10, QChar('0'));

            childColumns << new QStandardItem("新建章节") << new QStandardItem(childCode);
            chaptersModel->appendRow(childColumns);
        }
    });

    // 删除章节
    connect(deleteAction, &QAction::triggered, [this] {
        if (!currentChapterIndex.isValid()) { return; }

        QModelIndex nameIndex = chaptersModel->index(currentChapterIndex.row(), 0, currentChapterIndex.parent());
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
void BookEditor::openBook(const QString &path) {
    QFile file(path);
    if (!file.exists()) {
        // 选中的教材的 code 不为空时提示
        if (currentBookIndex.isValid() && !currentBookIndex.data(ROLE_CODE).toString().isEmpty()) {
            MessageBox::message(QString("教材 %1 不存在").arg(path));
        }

        return;
    }

    Json json(path, true);
    ui->bookCoverEdit->setText(json.getString("cover"));
    ui->bookRequirementEdit->setText(json.getString("requirement"));
    // ui->bookCodeEdit->setText(json.getString("code"));       // 编码由左侧的教材树提供
    // ui->bookSubjectEdit->setText(json.getString("subject")); // 学科由左侧的教材树提供
    // ui->bookVersionEdit->setText(json.getString("version")); // 版本由左侧的教材树提供

    bookService->openChapters(json);
    ui->chaptersTreeView->expandAll();
    originalBookCode = ui->bookCodeEdit->text().trimmed(); // 保存打开时候的教材编码
}

// 打开教材结构显示到左侧的教材目录树中
void BookEditor::openBooks(const QString &path) {
    resetBook();
    bookService->openBooks(path);
    ui->booksTreeView->expandAll();
}

// 重置右边的教材信息
void BookEditor::resetBook() {
    ui->bookCodeEdit->setText("");
    ui->bookSubjectEdit->setText("");
    ui->bookCoverEdit->setText("");
    ui->bookVersionEdit->setText("");
    ui->bookRequirementEdit->setText("");
    chaptersModel->removeRows(0, chaptersModel->rowCount());
}

// 校验教材和章节的编码是否唯一未被重复使用
bool BookEditor::validate() const {
    // 校验逻辑:
    // 1. 没选择教材则只校验教材结构
    // 2. 选择教材不但要校验教材结构，还要校验教材内容
    //    2.1 教材的学科不能为空
    //    2.2 教材的版本不能为空
    //    2.3 教材的编码不能为空
    //    2.4 教材编码不能重复使用
    //    2.5 章节编码不能在当前教材中重复使用

    if (!bookService->isBookIndex(currentBookIndex)) {
        // [1] 没选择教材则只校验教材结构
        QString error;
        bool ok = bookService->validateBooks(currentBookIndex, ui->bookCodeEdit->text().trimmed(), &error);

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

        // [2.3] 教材的编码不能为空
        if (ui->bookCodeEdit->text().trimmed().isEmpty()) {
            MessageBox::message("教材的 <font color='red'>编码</font> 不能为空");
            return false;
        }

        // [2.4] 教材编码不能重复使用
        // [2.5] 章节编码不能在当前教材中重复使用
        QString error1;
        QString error2;
        bool ok1 = bookService->validateBooks(currentBookIndex, ui->bookCodeEdit->text().trimmed(), &error1);
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

    if (!bookService->isBookIndex(currentBookIndex)) {
        // [2] 没选择教材则只保存教材结构到 books.json
        bool ok = bookService->saveBooks(booksDir);

        if (ok) {
            MessageBox::message("<font color='green'>保存成功</fong>");
        } else {
            MessageBox::message("<font color='red'>保存失败</fong>");
        }
    } else {
        // [3] 选择教材不但要保存教材结构，还要保存教材内容
        QString bookCode        = ui->bookCodeEdit->text().trimmed();
        QString bookSubject     = ui->bookSubjectEdit->text().trimmed();
        QString bookVersion     = ui->bookVersionEdit->text().trimmed();
        QString bookCover       = ui->bookCoverEdit->text().trimmed();
        QString bookRequirement = ui->bookRequirementEdit->text().trimmed();

        // [3.1] 更新 bookCode 到教材结构中选中的教材
        booksModel->setData(currentBookIndex, bookCode, ROLE_CODE);

        // [3.2] 删除旧的教材文件 ${originalBookCode}.json
        if (originalBookCode != bookCode) {
            QFile::remove(booksDir.filePath(originalBookCode + ".json"));
        }
        originalBookCode = bookCode;

        // [3.3] 保存教材结构到 books.json
        // [3.4] 保存当前的教材到 ${bookCode}.json
        bool ok1 = bookService->saveBooks(booksDir);
        bool ok2 = bookService->saveBook(bookCode, bookSubject, bookVersion, bookRequirement, bookCover, booksDir);

        if (ok1 && ok2) {
            MessageBox::message("<font color='green'>保存成功</fong>");
        } else {
            MessageBox::message("<font color='red'>保存失败</fong>");
        }
    }
}

