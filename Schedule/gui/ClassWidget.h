#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class ClassWidget;
}

class QSqlTableModel;

/**
 * @brief 管理班级的类，操作数据库中的表 class
 */
class ClassWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClassWidget(QWidget *parent = 0);
    ~ClassWidget();

private:
    void initializeModel();
    void handleEvents();

    Ui::ClassWidget *ui;
    QSqlTableModel *model;
    QSqlDatabase db;
};

#endif // CLASSWIDGET_H
