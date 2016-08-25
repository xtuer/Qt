#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

#include <QDialog>

struct NotesDialogPrivate;
template<typename T> class QList;
template<typename T1, typename T2> struct QPair;

namespace Ui {
class NotesDialog;
}

class NotesDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotesDialog(QWidget *parent = 0);
    ~NotesDialog();

    QList<QPair<QString, QString> > getNotes();
    void setNotes(const QList<QPair<QString, QString> > &notes);
    QList<QPair<QString, QString> > getEncodedNotes();
    void setEncodedNotes(const QList<QPair<QString, QString> > &notes);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void save();
    void open();
    void append();
    void remove();
    void changeEditMode(bool editable);
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void notesChanged();

private:
    void initTable();
    void createConnections();
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);

    QString getNoteTitle(int row) const;
    QString getNoteContent(int row) const;
    void setNoteTitle(int row, const QString &text);
    void setNoteContent(int row, const QString &content);

    void testData();
    void readSettings();
    void writeSettings();

    Ui::NotesDialog *ui;
    NotesDialogPrivate *data;
};

#endif // NOTESDIALOG_H
