#ifndef MANAGECOURSEWIDGET_H
#define MANAGECOURSEWIDGET_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
class ManageCourseWidget;
}

class ManageCourseWidget : public QWidget {
    Q_OBJECT

public:
    explicit ManageCourseWidget(QWidget *parent = 0);
    ~ManageCourseWidget();

private slots:
    void insert();
    void remove();
    void cancel();
    void save();

private:
    Ui::ManageCourseWidget *ui;
    QSqlTableModel *model;
};

#endif // MANAGECOURSEWIDGET_H
