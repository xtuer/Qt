#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog {
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();

private slots:
    void savePassword();
    void savePath();
    void browse();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H
