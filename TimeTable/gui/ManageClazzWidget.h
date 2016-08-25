#ifndef MANAGECLAZZWIDGET_H
#define MANAGECLAZZWIDGET_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
class ManageClazzWidget;
}

class ManageClazzWidget : public QWidget {
    Q_OBJECT

public:
    explicit ManageClazzWidget(QWidget *parent = 0);
    ~ManageClazzWidget();

private slots:
    void insert();
    void remove();
    void cancel();
    void save();

private:
    Ui::ManageClazzWidget *ui;
    QSqlTableModel *model;
};

#endif // MANAGECLAZZWIDGET_H
