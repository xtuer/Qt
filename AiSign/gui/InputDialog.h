#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

/**
 * 获取考生姓名和考籍号的对话框
 */
class InputDialog : public QDialog {
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = 0);
    ~InputDialog();

    QString getExamineeName() const; // 考生姓名
    QString getExamUid() const;      // 考籍号

private:
    void handleEvents();
    Ui::InputDialog *ui;

    QString examineeName;
    QString examUid;
};

#endif // INPUTDIALOG_H
