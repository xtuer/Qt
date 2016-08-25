#include "NotesDialog.h"
#include "ui_NotesDialog.h"
#include "../util/Global.h"
#include "../util/Settings.h"
#include "../util/CommonUtil.h"

#include <QList>
#include <QPair>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QCloseEvent>

struct NotesDialogPrivate {
    QStandardItemModel *model;
};

NotesDialog::NotesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NotesDialog),
    data(new NotesDialogPrivate) {
    ui->setupUi(this);
    initTable();

    createConnections();

    readFile(Global::FILE_NAME_NOTES);
    ui->editModeCheckBox->setChecked(false);
    ui->tableView->setCurrentIndex(data->model->index(0, 0));

    readSettings();
}

NotesDialog::~NotesDialog() {
    writeSettings();

    delete ui;
}

void NotesDialog::initTable() {
    data->model = new QStandardItemModel(0, 1, this);
    data->model->setHorizontalHeaderLabels(QStringList() << tr("标题"));
    ui->tableView->setModel(data->model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->verticalHeader()->hide();
}

void NotesDialog::createConnections() {
    connect(ui->saveButton,   SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->openButton,   SIGNAL(clicked()), this, SLOT(open()));
    connect(ui->appendButton, SIGNAL(clicked()), this, SLOT(append()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->editModeCheckBox, SIGNAL(toggled(bool)), this, SLOT(changeEditMode(bool)));
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(notesChanged()));
    connect(data->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(notesChanged()));
}

void NotesDialog::changeEditMode(bool editable) {
    ui->saveButton->setEnabled(editable);
    ui->openButton->setEnabled(editable);
    ui->appendButton->setEnabled(editable);
    ui->removeButton->setEnabled(editable);
    ui->textEdit->setReadOnly(!editable);
    ui->tableView->setEditTriggers(editable ? QAbstractItemView::DoubleClicked : QAbstractItemView::NoEditTriggers);
}

void NotesDialog::append() {
    data->model->appendRow(QList<QStandardItem*>());

    QModelIndex index = data->model->index(data->model->rowCount() - 1, 0);
    ui->tableView->setCurrentIndex(index);
    ui->tableView->edit(index);
}

void NotesDialog::remove() {
    QModelIndex index = ui->tableView->currentIndex();

    if (index.isValid()) {
        int result = QMessageBox::question(this, tr("删除笔记"), tr("确定删除笔记 ?"), QMessageBox::Yes | QMessageBox::No);

        if (QMessageBox::Yes == result) {
            data->model->removeRow(index.row());
        }
    }
}

void NotesDialog::save() {
    int result = QMessageBox::question(this, tr("保存"), tr("进行保存 ?"), QMessageBox::Yes | QMessageBox::No);

    if (QMessageBox::Yes == result) {
        writeFile(Global::FILE_NAME_NOTES);
        setWindowModified(false);
    }
}

void NotesDialog::open() {
    QString path = QFileDialog::getOpenFileName(this, tr("打开笔记"),
                                                QApplication::applicationDirPath(),
                                                tr("笔记 (*.dat)"));

    if (!path.isEmpty()) {
        readFile(path);
    }
}

void NotesDialog::currentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
    setNoteContent(previous.row(), ui->textEdit->toPlainText());
    ui->textEdit->setPlainText(getNoteContent(current.row()));
}

bool NotesDialog::readFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             tr("打开笔记"),
                             tr("打不开笔记文件 %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_1);
    quint32 magic;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    in >> magic;

    if (magic != Global::MAGIC_NUMBER_NOTES) {
        QMessageBox::warning(this, tr("打开笔记"), tr("文件格式不对."));
        QApplication::restoreOverrideCursor();
        return false;
    }

    QList<QPair<QString, QString> > notes;
    in >> notes;
//    setNotes(notes);
    setEncodedNotes(notes);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->setFocus();
    QApplication::restoreOverrideCursor();

    return true;
}

bool NotesDialog::writeFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("MainWidget"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_1);

    // Write the application's magic number and the talbe's row as well column count.
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << quint32(Global::MAGIC_NUMBER_NOTES) << getEncodedNotes();
    QApplication::restoreOverrideCursor();

    return true;
}

QList<QPair<QString, QString> > NotesDialog::getNotes() {
    // Save the current no saved text.
    setNoteContent(ui->tableView->currentIndex().row(), ui->textEdit->toPlainText());

    QList<QPair<QString, QString> > notes;
    const int rowCount = data->model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QString title = getNoteTitle(row);
        QString content = getNoteContent(row);
        notes << qMakePair(title, content);
    }

    return notes;
}

void NotesDialog::setNotes(const QList<QPair<QString, QString> > &notes) {
    // Clear the table.
    data->model->removeRows(0, data->model->rowCount());
    data->model->setRowCount(notes.size());

    const int rowCount = notes.size();

    for (int row = 0; row < rowCount; ++row) {
        QPair<QString, QString> note = notes.at(row);
        setNoteTitle(row, note.first);
        setNoteContent(row, note.second);
    }
}

QList<QPair<QString, QString> > NotesDialog::getEncodedNotes() {
    // Save the current no saved text.
    setNoteContent(ui->tableView->currentIndex().row(), ui->textEdit->toPlainText());

    QList<QPair<QString, QString> > notes;
    const int rowCount = data->model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QString title = CommonUtil::encodeString(getNoteTitle(row));
        QString content = CommonUtil::encodeString(getNoteContent(row));
        notes << qMakePair(title, content);
    }

    return notes;
}

void NotesDialog::setEncodedNotes(const QList<QPair<QString, QString> > &notes) {
    // Clear the table.
    data->model->removeRows(0, data->model->rowCount());
    data->model->setRowCount(notes.size());

    const int rowCount = notes.size();

    for (int row = 0; row < rowCount; ++row) {
        QPair<QString, QString> note = notes.at(row);
        setNoteTitle(row, CommonUtil::decodeString(note.first));
        setNoteContent(row, CommonUtil::decodeString(note.second));
    }
}

QString NotesDialog::getNoteTitle(int row) const {
    QModelIndex index = data->model->index(row, 0);
    return data->model->data(index).toString();
}

QString NotesDialog::getNoteContent(int row) const {
    QModelIndex index = data->model->index(row, 0);
    return data->model->data(index, Qt::UserRole).toString();
}

void NotesDialog::setNoteTitle(int row, const QString &title) {
    QModelIndex index = data->model->index(row, 0);
    data->model->setData(index, title);
}

void NotesDialog::setNoteContent(int row, const QString &content) {
    QModelIndex index = data->model->index(row, 0);
    data->model->setData(index, content, Qt::UserRole);
}

void NotesDialog::testData() {
    QList<QPair<QString, QString> > notes;
    notes << qMakePair(QString("1"), QString("1"));
    notes << qMakePair(QString("2"), QString("2"));
    notes << qMakePair(QString("3"), QString("3"));
    setNotes(notes);
}

void NotesDialog::readSettings() {
    Settings::getInstance().restoreWindowBounds(this, objectName());
    Settings::getInstance().restoreSplitterState(ui->splitter, ui->splitter->objectName());
}

void NotesDialog::writeSettings() {
    Settings::getInstance().saveSplitterState(*ui->splitter, ui->splitter->objectName());
    Settings::getInstance().saveWindowBounds(*this, objectName());
}

void NotesDialog::notesChanged() {
//    setWindowModified(true);
}

void NotesDialog::closeEvent(QCloseEvent *event) {
    if (ui->editModeCheckBox->isChecked()) {
        int result = QMessageBox::question(this, tr("保存"), tr("修改未保存，确定退出吗 ?"), QMessageBox::Yes | QMessageBox::No);

        if (QMessageBox::Yes == result) {
            save();
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void NotesDialog::keyPressEvent(QKeyEvent *event) {
    if(event->key() != Qt::Key_Escape) QDialog::keyPressEvent(event);
}
