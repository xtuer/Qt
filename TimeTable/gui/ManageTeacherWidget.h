#ifndef MANAGETEACHERWIDGET_H
#define MANAGETEACHERWIDGET_H

#include <QWidget>

class QSqlTableModel;
class TimeTableData;

namespace Ui {
class ManageTeacherWidget;
}

class ManageTeacherWidget : public QWidget {
    Q_OBJECT

public:
    explicit ManageTeacherWidget(QWidget *parent = 0);
    ~ManageTeacherWidget();

private slots:
    void insert();
    void remove();
    void cancel();
    void save();

private:
    Ui::ManageTeacherWidget *ui;
    QSqlTableModel *model;
};

#endif // MANAGETEACHERWIDGET_H
