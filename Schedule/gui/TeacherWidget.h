#ifndef TEACHERWIDGET_H
#define TEACHERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QSqlTableModel;

namespace Ui {
class TeacherWidget;
}

class TeacherWidget : public QWidget {
    Q_OBJECT

public:
    explicit TeacherWidget(QWidget *parent = 0);
    ~TeacherWidget();

private:
    void initializeModel();
    void handleEvents();

    Ui::TeacherWidget *ui;
    QSqlTableModel *model;
    QSqlDatabase db;
};

#endif // TEACHERWIDGET_H
