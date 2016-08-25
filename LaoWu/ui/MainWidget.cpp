#include "MainWidget.h"
#include "NotesDialog.h"
#include "ui_MainWidget.h"
#include "EditDialog.h"
#include "SetupDialog.h"
#include "delegate/DateItemDelegate.h"
#include "delegate/ComboBoxItemDelegate.h"
#include "delegate/ValidatorItemDelegate.h"
#include "delegate/CutDownItemDelegate.h"
#include "../domain/UserData.h"
#include "../util/Global.h"
#include "../util/Settings.h"
#include "../model/SortModel.h"

#include <QDate>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QClipboard>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QContextMenuEvent>

struct MainWidgetPrivate {
    SortModel          *selectedSortModel;
    SortModel          *usingSortModel;
    SortModel          *stoppedSortModel;
    QStandardItemModel *usingModel;
    QStandardItemModel *stoppedModel;

    QAction *copyAction;
    QAction *searchAction;
};

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    data = new MainWidgetPrivate;

    setupTableView();
    createActions();
    createDelegates();
    createConnections();

    ui->editModeCheckBox->setChecked(false);

    readFile(Global::FILE_NAME_DATA);
    readSettings();
}

MainWidget::~MainWidget() {
    writeSettings();

    delete ui;
    delete data;
}

bool MainWidget::eventFilter(QObject *watched, QEvent *event) {
    if (QEvent::FocusIn == event->type()) {
        if (watched == ui->usingTableView) {
            data->selectedSortModel = data->usingSortModel;
        } else if (watched == ui->stoppedTableView) {
            data->selectedSortModel = data->stoppedSortModel;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainWidget::setupTableView() {
    data->selectedSortModel = NULL;

    // Init using models and table view.
    data->usingModel = new QStandardItemModel(this);
    data->usingModel->setHorizontalHeaderLabels(UserData::fieldLabels);
    data->usingModel->setRowCount(1);
    data->usingSortModel = new SortModel(this);
    data->usingSortModel->setSourceModel(data->usingModel);

    ui->usingTableView->setModel(data->usingSortModel);
    ui->usingTableView->setSortingEnabled(true);
//    ui->usingTableView->horizontalHeader()->setSectionsMovable(true);
    ui->usingTableView->horizontalHeader()->setSectionsClickable(true);
    ui->usingTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usingTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->usingTableView->setEditTriggers(QTableView::NoEditTriggers);

    // Init stopped models and table view.
    data->stoppedModel = new QStandardItemModel(this);
    data->stoppedModel->setHorizontalHeaderLabels(UserData::fieldLabels);
    data->stoppedModel->setRowCount(1);
    data->stoppedSortModel = new SortModel(this);
    data->stoppedSortModel->setSourceModel(data->stoppedModel);

    ui->stoppedTableView->setModel(data->stoppedSortModel);
    ui->stoppedTableView->setSortingEnabled(true);
//    ui->stoppedTableView->horizontalHeader()->setSectionsMovable(true);
    ui->stoppedTableView->horizontalHeader()->setSectionsClickable(true);
    ui->stoppedTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->stoppedTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->stoppedTableView->setEditTriggers(QTableView::NoEditTriggers);

    // install event filter
    ui->usingTableView->installEventFilter(this);
    ui->stoppedTableView->installEventFilter(this);
}

void MainWidget::createActions() {
    // Copy action
    data->copyAction = new QAction(this);
    data->copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    this->addAction(data->copyAction);

    // Search Action
    data->searchAction = new QAction(this);
    data->searchAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    this->addAction(data->searchAction);
}

void MainWidget::createDelegates() {
//    DoubleItemDelegate *doubleDelegate = new DoubleItemDelegate(tr(""),    Qt::AlignRight|Qt::AlignVCenter, this);
//    DoubleItemDelegate *kapitalDelegate = new DoubleItemDelegate(tr("万"), Qt::AlignRight|Qt::AlignVCenter, this);
//    DoubleItemDelegate *interestDelegate = new DoubleItemDelegate(tr("%"), Qt::AlignRight|Qt::AlignVCenter, this);

//    QStringList types;
//    types << tr("股票") << tr("基金") << tr("民贷") << tr("自定");
//    ComboBoxItemDelegate *comboBoxDelegate = new ComboBoxItemDelegate(types, this);

//    DateItemDelegate *startDateDelegate = new DateItemDelegate(QDate::currentDate(), this);
//    DateItemDelegate *endDateDelegate = new DateItemDelegate(QDate::currentDate().addMonths(1), this);

//    // Set the table view's delegates.
//    ui->tableView->setItemDelegateForColumn(0, comboBoxDelegate);
//    ui->tableView->setItemDelegateForColumn(5, kapitalDelegate);
//    ui->tableView->setItemDelegateForColumn(8, startDateDelegate);
//    ui->tableView->setItemDelegateForColumn(9, endDateDelegate);
//    ui->tableView->setItemDelegateForColumn(10, interestDelegate);
//    ui->tableView->setItemDelegateForColumn(11, interestDelegate);
//    ui->tableView->setItemDelegateForColumn(12, interestDelegate);
//    ui->tableView->setItemDelegateForColumn(14, doubleDelegate);
//    ui->tableView->setItemDelegateForColumn(15, doubleDelegate);
    CutDownItemDelegate *cdid = new CutDownItemDelegate(this);
    ui->usingTableView->setItemDelegateForColumn(6, cdid);
    ui->stoppedTableView->setItemDelegateForColumn(6, cdid);
}

void MainWidget::createConnections() {
    connect(ui->setupButton,        SIGNAL(clicked()),      this, SLOT(openSetupDialog()));
    connect(data->copyAction,       SIGNAL(triggered()),    this, SLOT(copyPointedIndexText()));
    connect(data->searchAction,     SIGNAL(triggered()),    this, SLOT(focusSearchLineEdit()));
    connect(ui->searchLineEdit,     SIGNAL(returnPressed()),this, SLOT(search()));
    connect(ui->searchButton,       SIGNAL(clicked()),      this, SLOT(search()));
    connect(ui->notesButton,        SIGNAL(clicked()),      this, SLOT(openNotesDialog()));
    connect(ui->importButton,       SIGNAL(clicked()),      this, SLOT(import()));
    connect(ui->exportButton,       SIGNAL(clicked()),      this, SLOT(exportHtml()));
    connect(ui->editModeCheckBox,   SIGNAL(toggled(bool)),  this, SLOT(changeEditMode(bool)));
    connect(ui->appendRowButton,    SIGNAL(clicked()),      this, SLOT(openCreateDialog()));
    connect(ui->removeRowButton,    SIGNAL(clicked()),      this, SLOT(removeRow()));
    connect(ui->usingTableView,     SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openEditDialog(QModelIndex)));
    connect(ui->stoppedTableView,   SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openEditDialog(QModelIndex)));

    connect(data->usingModel,   SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,               SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    connect(data->stoppedModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,               SLOT(tableDataChanged(QModelIndex,QModelIndex)));

    // Change header label
    connect(ui->usingTableView->horizontalHeader(),   SIGNAL(sectionDoubleClicked(int)),
            this,                                     SLOT(changeHorizontalHeader(int)));
    connect(ui->stoppedTableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
            this,                                     SLOT(changeHorizontalHeader(int)));

    // Resize section
    connect(ui->usingTableView->horizontalHeader(),   SIGNAL(sectionResized(int,int,int)),
            this,                                     SLOT(resizeHeaderSection(int,int,int)));
    connect(ui->stoppedTableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            this,                                     SLOT(resizeHeaderSection(int,int,int)));

    // Update vertical header
    connect(data->usingSortModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateVerticalHeader()));
    connect(data->usingSortModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(updateVerticalHeader()));
    connect(data->usingSortModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateVerticalHeader()));

    connect(data->stoppedSortModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateVerticalHeader()));
    connect(data->stoppedSortModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(updateVerticalHeader()));
    connect(data->stoppedSortModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateVerticalHeader()));

    connect(ui->usingTableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(updateVerticalHeader()));
    connect(ui->stoppedTableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(updateVerticalHeader()));
}

void MainWidget::changeEditMode(bool editable) {
//    ui->saveButton->setEnabled(editable);
    ui->appendRowButton->setEnabled(editable);
    ui->removeRowButton->setEnabled(editable);
    ui->importButton->setEnabled(editable);
}

void MainWidget::removeRow() {
    QModelIndex index = data->selectedSortModel->mapToSource(getSelectedTableView()->currentIndex());

    if (index.isValid()) {
        int result = QMessageBox::question(this, tr("删除"), tr("确定指定行 ?"), QMessageBox::Yes | QMessageBox::No);

        if (QMessageBox::Yes == result) {
            getSelectedModel()->removeRow(index.row());
            save();
        }
    }
}

void MainWidget::changeHorizontalHeader(int logicalIndex) {
    if (logicalIndex < 18) { return; }

    QString str = QInputDialog::getText(this, "Header", "Header");
    if (!str.isEmpty()) {
        data->usingModel->setHeaderData(logicalIndex, Qt::Horizontal, str, Qt::DisplayRole);
        data->stoppedModel->setHeaderData(logicalIndex, Qt::Horizontal, str, Qt::DisplayRole);
    }
}

void MainWidget::updateVerticalHeader() {
    int visualIndex = 0;
    const int usingRowCount = data->usingSortModel->rowCount();
    for (int row = 0; row < usingRowCount; ++row) {
        if (!ui->usingTableView->isRowHidden(row)) {
            ++visualIndex;
            int logicalIndex = ui->usingTableView->verticalHeader()->logicalIndex(row);
            data->usingSortModel->setHeaderData(logicalIndex, Qt::Vertical, visualIndex);
        }
    }

    visualIndex = 0;
    const int stoppedRowCount = data->stoppedSortModel->rowCount();
    for (int row = 0; row < stoppedRowCount; ++row) {
        if (!ui->stoppedTableView->isRowHidden(row)) {
            ++visualIndex;
            int logicalIndex = ui->stoppedTableView->verticalHeader()->logicalIndex(row);
            data->stoppedSortModel->setHeaderData(logicalIndex, Qt::Vertical, visualIndex);
        }
    }
}

bool MainWidget::save() {
    return writeFile(Global::FILE_NAME_DATA);
}

bool MainWidget::writeFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("主窗口"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_1);

    const quint32 usingRowCount   = data->usingModel->rowCount();
    const quint32 usingColCount   = data->usingModel->columnCount();
    const quint32 stoppedRowCount = data->stoppedModel->rowCount();
    const quint32 stoppedColCount = data->stoppedModel->columnCount();

    // Write the application's magic number and the talbe's row as well column count.
    out << quint32(Global::MAGIC_NUMBER_DATA)
        << usingRowCount   << usingColCount
        << stoppedRowCount << stoppedColCount;

    for (int row = 0; row < usingRowCount; ++row) {
        for (int col = 0; col < usingColCount; ++col) {
            out << getItemDataString(row, col, *data->usingModel);
        }
    }

    for (int row = 0; row < stoppedRowCount; ++row) {
        for (int col = 0; col < stoppedColCount; ++col) {
            out << getItemDataString(row, col, *data->stoppedModel);
        }
    }

    saveHorizontalHeaderState(out);
    QApplication::restoreOverrideCursor();

    return true;
}

bool MainWidget::readFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             tr("打开数据文件"),
                             tr("打不开数据文件 %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_1);

    quint32 magic;
    in >> magic;

    if (magic != Global::MAGIC_NUMBER_DATA) {
        QMessageBox::warning(this, tr("数据文件"), tr("文件格式不对."));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    quint32 usingRowCount   = 0;
    quint32 usingColCount   = 0;
    quint32 stoppedRowCount = 0;
    quint32 stoppedColCount = 0;

    QString str;

    in >> usingRowCount >> usingColCount >> stoppedRowCount >> stoppedColCount;

    data->usingModel->removeRows(0, data->usingModel->rowCount());
    data->usingModel->setRowCount(usingRowCount);
    data->usingModel->setColumnCount(usingColCount);
    data->stoppedModel->removeRows(0, data->stoppedModel->rowCount());
    data->stoppedModel->setRowCount(stoppedRowCount);
    data->stoppedModel->setColumnCount(stoppedColCount);

    for (int row = 0; row < usingRowCount; ++row) {
        for (int col = 0; col < usingColCount; ++col) {
            in >> str;
            setItemDataString(row, col, str, data->usingModel);
        }
    }

    for (int row = 0; row < stoppedRowCount; ++row) {
        for (int col = 0; col < stoppedColCount; ++col) {
            in >> str;
            setItemDataString(row, col, str, data->stoppedModel);
        }
    }

    restoreHorizontalHeaderState(in);
    showAccumulate();
    QApplication::restoreOverrideCursor();

    return true;
}

void MainWidget::saveHorizontalHeaderState(QDataStream &out) {
    const int columnCount = data->usingModel->columnCount();

    out << ui->usingTableView->horizontalHeader()->saveState();

    for (int col = 0; col < columnCount; ++col) {
        QString label = data->usingModel->horizontalHeaderItem(col)->data(Qt::DisplayRole).toString();
        out << quint32(col) << label;
    }
}

void MainWidget::restoreHorizontalHeaderState(QDataStream &in) {
    QByteArray state;
    in >> state;
    //ui->usingTableView->horizontalHeader()->restoreState(state);
    //ui->stoppedTableView->horizontalHeader()->restoreState(state);

    const int columnCount = data->usingModel->columnCount();

    for (int col = 0; col < columnCount; ++col) {
        quint32 column;
        QString label;
        in >> column >> label;

        data->usingModel->setHorizontalHeaderItem(column, new QStandardItem(label));
        data->stoppedModel->setHorizontalHeaderItem(column, new QStandardItem(label));
    }
}

void MainWidget::openNotesDialog() {
    NotesDialog *notesDialog = new NotesDialog(this);
    notesDialog->setAttribute(Qt::WA_DeleteOnClose);

    notesDialog->show();
    notesDialog->raise();
    notesDialog->setFocus();
}

void MainWidget::openSetupDialog() {
    SetupDialog setupDialog;
    setupDialog.exec();
}

void MainWidget::openCreateDialog() {
    EditDialog dlg(tr("添加"), this);
    dlg.setReservedLabels(getReservedLabels());

    if (QDialog::Accepted == dlg.exec()) {
        UserData ud = dlg.getUserData();
        QStandardItemModel *model = (tr("在用") == ud.zhuangTai) ? data->usingModel : data->stoppedModel;

        model->appendRow(QList<QStandardItem*>());
        setUserData(model->rowCount() - 1, ud, model);

        save();
        showAccumulate();
    }
}

void MainWidget::openEditDialog(const QModelIndex &proxyIndex) {
    QModelIndex index = data->selectedSortModel->mapToSource(proxyIndex);
    if (!index.isValid() || index.column() != 6 || !ui->editModeCheckBox->isChecked()) { return; }

    EditDialog dlg(tr("编辑"), this);
    dlg.setUserData(getUserData(index.row(), *getSelectedModel()));
    dlg.setReservedLabels(getReservedLabels());

    if (QDialog::Accepted == dlg.exec()) {
        UserData ud = dlg.getUserData();
        QStandardItemModel *model = (tr("在用") == ud.zhuangTai) ? data->usingModel : data->stoppedModel;

        if (model != data->selectedSortModel->sourceModel()) {
            data->selectedSortModel->sourceModel()->removeRow(index.row());
            model->appendRow(QList<QStandardItem*>());
            setUserData(model->rowCount() - 1, ud, model);
        } else {
            setUserData(index.row(), ud, model);
        }

        save();
        showAccumulate();
    }
}

void MainWidget::setUserData(int row, const UserData &userData, QStandardItemModel *model) {
    setItemDataString(row, 0,  userData.leiXin, model);
    setItemDataString(row, 1,  userData.shangQuan, model);
    setItemDataString(row, 2,  userData.guiShu, model);
    setItemDataString(row, 3,  userData.zhangHao, model);
    setItemDataString(row, 4,  userData.jiaoYiMiMa, model);
    setItemDataString(row, 5,  userData.benJing, model);
    setItemDataString(row, 6,  userData.huMing, model);
    // setItemDataString(row, 7,  userData.daoJiShi);
    setItemDataString(row, 8,  userData.kaiShiRi, model);
    setItemDataString(row, 9,  userData.zhiFuDao, model);
    setItemDataString(row, 10, userData.liXi, model);
    setItemDataString(row, 11, userData.yinFuLiXi, model);
    setItemDataString(row, 12, userData.ShiFuLiXi, model);
    setItemDataString(row, 13, userData.zhiJingMiMa, model);
    setItemDataString(row, 14, userData.biLi, model);
    setItemDataString(row, 15, userData.yongJing, model);
    setItemDataString(row, 16, userData.zhuangTai, model);
    setItemDataString(row, 17, userData.beiZhu, model);

    setItemDataString(row, 18, userData.reserved1, model);
    setItemDataString(row, 19, userData.reserved2, model);
    setItemDataString(row, 20, userData.reserved3, model);
    setItemDataString(row, 21, userData.reserved4, model);
    setItemDataString(row, 22, userData.reserved5, model);
    setItemDataString(row, 23, userData.reserved6, model);
}

UserData MainWidget::getUserData(int row, const QStandardItemModel &model) const {
    UserData userData;

    userData.leiXin     = getItemDataString(row, 0, model);
    userData.shangQuan  = getItemDataString(row, 1, model);
    userData.guiShu     = getItemDataString(row, 2, model);
    userData.zhangHao   = getItemDataString(row, 3, model);
    userData.jiaoYiMiMa = getItemDataString(row, 4, model);
    userData.benJing    = getItemDataString(row, 5, model);
    userData.huMing     = getItemDataString(row, 6, model);
    userData.daoJiShi   = getItemDataString(row, 7, model);
    userData.kaiShiRi   = getItemDataString(row, 8, model);
    userData.zhiFuDao   = getItemDataString(row, 9, model);
    userData.liXi       = getItemDataString(row, 10, model);
    userData.yinFuLiXi  = getItemDataString(row, 11, model);
    userData.ShiFuLiXi  = getItemDataString(row, 12, model);
    userData.zhiJingMiMa= getItemDataString(row, 13, model);
    userData.biLi       = getItemDataString(row, 14, model);
    userData.yongJing   = getItemDataString(row, 15, model);
    userData.zhuangTai  = getItemDataString(row, 16, model);
    userData.beiZhu     = getItemDataString(row, 17, model);

    userData.reserved1 = getItemDataString(row, 18, model);
    userData.reserved2 = getItemDataString(row, 19, model);
    userData.reserved3 = getItemDataString(row, 20, model);
    userData.reserved4 = getItemDataString(row, 21, model);
    userData.reserved5 = getItemDataString(row, 22, model);
    userData.reserved6 = getItemDataString(row, 23, model);

    return userData;
}

QString MainWidget::getItemDataString(int row, int column, const QStandardItemModel &model) const {
    return model.index(row, column).data().toString();
}

void MainWidget::setItemDataString(int row, int column, const QString &str, QStandardItemModel *model) {
    model->setData(model->index(row, column), str);
}

QStringList MainWidget::getReservedLabels() const {
    QStringList labels;
    QStandardItemModel *model = getSelectedModel();

    labels << model->headerData(18, Qt::Horizontal).toString();
    labels << model->headerData(19, Qt::Horizontal).toString();
    labels << model->headerData(20, Qt::Horizontal).toString();
    labels << model->headerData(21, Qt::Horizontal).toString();
    labels << model->headerData(22, Qt::Horizontal).toString();
    labels << model->headerData(23, Qt::Horizontal).toString();

    return labels;
}

void MainWidget::tableDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(sender());
    const int watchedColumn = 9;
    const int changedColumn = 7;

    if (topLeft.column() <= watchedColumn && watchedColumn <= bottomRight.column()) {
        const int startRow = topLeft.row();
        const int endRow = bottomRight.row();

        for (int row = startRow; row <= endRow; ++row) {
            QString dateString = getItemDataString(row, watchedColumn, *model);
            int days = QDate::currentDate().daysTo(QDate::fromString(dateString, Global::DATE_FORMAT));
            setItemDataString(row, changedColumn, QString::number(days) + tr("天"), model);
        }
    }
}

void MainWidget::copyPointedIndexText() {
    QPoint pos = getSelectedTableView()->viewport()->mapFromGlobal(QCursor::pos());
    QModelIndex index = getSelectedTableView()->indexAt(pos);
    QModelIndex indexSource = data->selectedSortModel->mapToSource(index);

    if (indexSource.isValid()) {
        QString text = getItemDataString(indexSource.row(), indexSource.column(), *getSelectedModel());
        QApplication::clipboard()->setText(text);
    }
}

void MainWidget::focusSearchLineEdit() {
    ui->searchLineEdit->setFocus();
}

void MainWidget::resizeHeaderSection(int logicalIndex, int oldSize, int newSize) {
    Q_UNUSED(oldSize);
    ui->usingTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
    ui->stoppedTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
}

void MainWidget::search() {
    QString text = ui->searchLineEdit->text().trimmed();
    const int usingRowCount = data->usingModel->rowCount();
    const int usingColCount = data->usingModel->columnCount();
    const int stoppedRowCount = data->stoppedModel->rowCount();
    const int stoppedColCount = data->stoppedModel->columnCount();

    // 排序影响的数据是sort model里的, 而原来的model中的数据的顺序是不变的.
    // table view的model是sort model

    // Show all rows.
    for (int row = 0; row < usingRowCount; ++row) {
        ui->usingTableView->showRow(row);
    }

    for (int row = 0; row < stoppedRowCount; ++row) {
        ui->stoppedTableView->showRow(row);
    }

    if (!text.isEmpty()) {
        for (int row = usingRowCount - 1; row >= 0; --row) {
            bool has = false;

            for (int col = 0; col < usingColCount; ++col) {
                QModelIndex sortedIndex = data->usingSortModel->index(row, col);
                QModelIndex index = data->usingSortModel->mapToSource(sortedIndex);
                QString str = getItemDataString(index.row(), index.column(), *data->usingModel);

                if (str.contains(text)) {
                    has = true;
                    break;
                }
            }

            if (!has) {
                ui->usingTableView->hideRow(row);
            }
        }

        for (int row = stoppedRowCount - 1; row >= 0; --row) {
            bool has = false;

            for (int col = 0; col < stoppedColCount; ++col) {
                QModelIndex sortedIndex = data->stoppedSortModel->index(row, col);
                QModelIndex index = data->stoppedSortModel->mapToSource(sortedIndex);
                QString str = getItemDataString(index.row(), index.column(), *data->stoppedModel);

                if (str.contains(text)) {
                    has = true;
                    break;
                }
            }

            if (!has) {
                ui->stoppedTableView->hideRow(row);
            }
        }
    }

    updateVerticalHeader();

    showAccumulate();
}

void MainWidget::import() {
    QString path = QFileDialog::getOpenFileName(this, tr("打开"),
                                                QApplication::applicationDirPath(),
                                                tr("数据文件 (*.dat)"));

    if (!path.isEmpty()) {
        readFile(path);
    }
}

QTableView* MainWidget::getSelectedTableView() const {
    return data->selectedSortModel == data->usingSortModel ? ui->usingTableView : ui->stoppedTableView;
}

QStandardItemModel* MainWidget::getSelectedModel() const {
    return data->selectedSortModel == data->usingSortModel ? data->usingModel : data->stoppedModel;
}

void MainWidget::readSettings() {
    Settings::getInstance().restoreWindowBounds(this, objectName());
    Settings::getInstance().restoreSplitterState(ui->splitter, objectName() + "/" + ui->splitter->objectName());
    Settings::getInstance().restoreHeaderState(ui->usingTableView->horizontalHeader(), objectName() + "/hHeader");
    Settings::getInstance().restoreHeaderState(ui->stoppedTableView->horizontalHeader(), objectName() + "/hHeader");
}

void MainWidget::writeSettings() {
    Settings::getInstance().saveHeaderState(*ui->usingTableView->horizontalHeader(), objectName() + "/hHeader");
    Settings::getInstance().saveSplitterState(*ui->splitter, objectName() + "/" + ui->splitter->objectName());
    Settings::getInstance().saveWindowBounds(*this, objectName());
}

void MainWidget::showAccumulate() {
    // 本金: 5
    // 应付利息: 11
    double benJingSum = 0;
    double yingFuLiXiSum = 0;

    const int usingRowCount = data->usingSortModel->rowCount();
    for (int row = 0; row < usingRowCount; ++row) {
        if (!ui->usingTableView->isRowHidden(row)) {
            QString b = data->usingSortModel->index(row, 5).data().toString();
            QString y = data->usingSortModel->index(row, 11).data().toString();

            benJingSum += Global::stringToDouble(b, QString("万"));
            yingFuLiXiSum += Global::stringToDouble(y, QString("万"));
        }
    }

    const int stoppedRowCount = data->stoppedSortModel->rowCount();
    for (int row = 0; row < stoppedRowCount; ++row) {
        if (!ui->stoppedTableView->isRowHidden(row)) {
            QString b = data->stoppedSortModel->index(row, 5).data().toString();
            QString y = data->stoppedSortModel->index(row, 11).data().toString();

            benJingSum += Global::stringToDouble(b, QString("万"));
            yingFuLiXiSum += Global::stringToDouble(y, QString("万"));
        }
    }

    ui->benJingHeJiLineEdit->setText(QString("%1 万").arg(QString::number(benJingSum)));
    ui->yingFuLiXiHeJiLineEdit->setText(QString("%1 万").arg(QString::number(yingFuLiXiSum)));
}

void MainWidget::exportHtml() {
    QString path = QFileDialog::getSaveFileName(this, tr("选择导出路径"), tr("./untitled.html"), tr("网页 (*.html)"));
    if (path.isEmpty()) { return; }

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("导出"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return;
    }

    QString html =
            "<html><head>"
            "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />"
            "<style>"
                "td, th{border: 1px solid gray; width: 100px; min-width: 120px}"
                "table {border-collapse:collapse}"
            "</style>"
            "</head>";

    html += "<body>\n";
    html += convertTableToHtml(*data->usingSortModel);
    html += "<br><br>";
    html += convertTableToHtml(*data->stoppedSortModel);
    html += "</body></html>";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << html;
    out.flush();
    file.close();
}

QString MainWidget::convertTableToHtml(const QSortFilterProxyModel &model) {
    QString html;

    const int rowCount = model.rowCount();
    const int colCount = model.columnCount();

    html += "<table>\n";

    // Header
    html += "<tr>\n";
    for (int col = 0; col < colCount; ++col) {
        QString label = getHeaderLabel(col, *ui->usingTableView->horizontalHeader(), model);
        html += QString("<th>%1</th>").arg(label.toHtmlEscaped());
    }
    html += "</tr>\n";

    // Td
    for (int row = 0; row < rowCount; ++row) {
        html += "<tr>\n";
        for (int col = 0; col < colCount; ++col) {
            html += QString("<td>%1</td>").arg(model.index(row, col).data().toString().toHtmlEscaped());
        }
        html += "</tr>\n";
    }
    html += "</table>\n";

    return html;
}

QString MainWidget::getHeaderLabel(int visualIndex, const QHeaderView &headerView, const QAbstractItemModel &model) {
    return model.headerData(headerView.logicalIndex(visualIndex), Qt::Horizontal).toString();
}
