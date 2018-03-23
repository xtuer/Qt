#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog {
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = 0);
    ~InputDialog();

    QString getExamineeName() const;
    QString getIdCardNo() const;

private:
    void handleEvents();
    Ui::InputDialog *ui;

    QString examineeName;
    QString idCardNo;
};

#endif // INPUTDIALOG_H
