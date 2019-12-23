#ifndef MANUALSIGNDIALOG_H
#define MANUALSIGNDIALOG_H

#include <QDialog>

namespace Ui {
class ManualSignDialog;
}

class ManualSignDialog : public QDialog {
    Q_OBJECT

public:
    explicit ManualSignDialog(QWidget *parent = nullptr);
    ~ManualSignDialog();

    QString getName() const {
        return name;
    }

    QString getExamNum() const {
        return examNum;
    }

    QString getSignCode() const {
        return signCode;
    }

private:
    Ui::ManualSignDialog *ui;

    QString name;     // 姓名
    QString examNum;  // 考籍号
    QString signCode; // 考期密码
};

#endif // MANUALSIGNDIALOG_H
