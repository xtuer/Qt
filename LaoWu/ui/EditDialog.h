#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class UserData;

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditDialog(const QString &title, QWidget *parent = 0);
    ~EditDialog();

    void setUserData(const UserData &userData);
    UserData getUserData() const;

    void setReservedLabels(const QStringList &labels);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void save();
    void changeEditMode(bool editable);

private:
    bool checkInput();
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
