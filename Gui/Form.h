#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT
public:
    Form(QWidget *parent = 0);

signals:
    void fox();

private:
    Ui::Form *ui;
};

#endif // FORM_H
