#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT
public:
    Form(QWidget *parent = 0);

private:
    Ui::Form *ui;
    QStandardItemModel *model;
};

#endif // FORM_H
