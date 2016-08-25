#ifndef TEACHERTIMETABLEWIDGET_H
#define TEACHERTIMETABLEWIDGET_H

#include <QWidget>

class QSqlTableModel;
class TimeTableData;
class QModelIndex;
template <typename T> class QList;

namespace Ui {
class TeacherTimeTableWidget;
}

class TeacherTimeTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit TeacherTimeTableWidget(QWidget *parent = 0);
    ~TeacherTimeTableWidget();

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    void loadTimeTableItems(const QModelIndex &current, const QModelIndex &previous);
    void exportTimeTableOfTeacher();
    void exportTimeTableOfAllTeachers();

private:
    void readTimeTableData();
    void exportTimeTableOfTeachers(const QList<int> teacherIds);

    Ui::TeacherTimeTableWidget *ui;
    TimeTableData *timeTableData;
    QSqlTableModel *model;
};

#endif // TEACHERTIMETABLEWIDGET_H
