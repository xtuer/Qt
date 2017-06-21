#ifndef COMMENTSETTINGSDIALOG_H
#define COMMENTSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class CommentSettingsDialog;
}

class CommentSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit CommentSettingsDialog(QWidget *parent = 0);
    ~CommentSettingsDialog();

private:
    Ui::CommentSettingsDialog *ui;
};

#endif // COMMENTSETTINGSDIALOG_H
