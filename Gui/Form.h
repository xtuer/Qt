#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void enterEvent(QEvent *event);

private:
    Ui::Form *ui;
};

#endif // FORM_H
