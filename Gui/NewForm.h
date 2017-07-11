#ifndef NEWFORM_H
#define NEWFORM_H

#include <QWidget>

namespace Ui {
class NewForm;
}

class NewForm : public QWidget {
    Q_OBJECT

public:
    explicit NewForm(QWidget *parent = 0);
    ~NewForm();

private:
    Ui::NewForm *ui;
};

#endif // NEWFORM_H
