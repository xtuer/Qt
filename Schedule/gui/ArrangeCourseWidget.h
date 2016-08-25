#ifndef ARRANGECOURSEWIDGET_H
#define ARRANGECOURSEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QSqlTableModel;
class QSqlRelationalTableModel;
class QShowEvent;

namespace Ui {
class ArrangeCourseWidget;
}

/**
 * @brief 课程安排
 */
class ArrangeCourseWidget : public QWidget {
    Q_OBJECT

public:
    explicit ArrangeCourseWidget(QWidget *parent = 0);
    ~ArrangeCourseWidget();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeClassModel();
    void initializeArrangeModel(int classId);
    void handleEvents();

    Ui::ArrangeCourseWidget *ui;
    QSqlTableModel *classModel;
    QSqlRelationalTableModel *arrangeModel;
    QSqlDatabase db;
    int currentClassId;
};

#endif // ARRANGECOURSEWIDGET_H
