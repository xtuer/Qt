#ifndef COURSEWIDGET_H
#define COURSEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class CourseWidget;
}

class QSqlTableModel;

/**
 * @brief 管理课程的类，操作数据库中的表 course
 */
class CourseWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseWidget(QWidget *parent = 0);
    ~CourseWidget();

private:
    void initializeModel();
    void handleEvents();

    Ui::CourseWidget *ui;
    QSqlTableModel *model;
    QSqlDatabase db;
};

#endif // COURSEWIDGET_H
