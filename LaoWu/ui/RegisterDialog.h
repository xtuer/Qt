#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
    class RegisterDialog;
}

/**
 * 程序注册窗口.
 */
class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();

    void checkRegister();

private slots:
    void registerSN();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
 
