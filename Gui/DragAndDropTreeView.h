#ifndef DRAGANDDROPTREEVIEW_H
#define DRAGANDDROPTREEVIEW_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class DragAndDropTreeView;
}

class DragAndDropTreeView : public QWidget {
    Q_OBJECT

public:
    explicit DragAndDropTreeView(QWidget *parent = 0);
    ~DragAndDropTreeView();

private:
    Ui::DragAndDropTreeView *ui;

    QStandardItemModel *model;
};

#endif // DRAGANDDROPTREEVIEW_H
