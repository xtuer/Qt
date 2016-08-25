#ifndef MANAGETIMETABLEWIDGET_H
#define MANAGETIMETABLEWIDGET_H

#include <QWidget>

class QDialog;
class TimeTableData;
class QSqlTableModel;

namespace Ui {
class ClazzTimeTableWidget;
}

class ClazzTimeTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClazzTimeTableWidget(QWidget *parent = 0);
    ~ClazzTimeTableWidget();

protected:
    virtual void showEvent(QShowEvent *);

private slots:
    void showCourseTeacherDialog();
    void loadTimeTableItems(const QModelIndex & current, const QModelIndex & previous);
    void saveTimeTableItems();
    void changeEditMode(bool editable);

    void exportTimeTableOfClazz();
    void exportTimeTableOfAllClazzes();

private:
    void loadClazzes();
    void readTimeTableData();
    Ui::ClazzTimeTableWidget *ui;
    TimeTableData *timeTableData;
    QSqlTableModel *model;
    QDialog *courseTeacherDialog;
};

#endif // MANAGETIMETABLEWIDGET_H
