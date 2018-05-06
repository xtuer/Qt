#ifndef ADDUSERWIDGET_H
#define ADDUSERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class AddUserWidget;
}

class AddUserWidget : public QWidget {
    Q_OBJECT

public:
    explicit AddUserWidget(QWidget *parent = 0);
    ~AddUserWidget();

private:
    void createUser();

    Ui::AddUserWidget *ui;
    QStandardItemModel *permissionsModel;
};

#endif // ADDUSERWIDGET_H
