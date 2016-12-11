#ifndef CODESTATUSWIDGET_H
#define CODESTATUSWIDGET_H

#include <QWidget>

namespace Ui {
class CodeStatusWidget;
}

class CodeStatusWidget : public QWidget {
    Q_OBJECT

public:
    explicit CodeStatusWidget(QWidget *parent = 0);
    ~CodeStatusWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::CodeStatusWidget *ui;
};

#endif // CODESTATUSWIDGET_H
