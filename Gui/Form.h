#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT
public:
    Form(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Form *ui;
    QPixmap pixmap;
};

#endif // FORM_H
