#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

struct MainWidgetPrivate;
template<typename T> class QList;
class UserData;
class QModelIndex;
class QStandardItemModel;
class QTableView;
class QHeaderView;
class QAbstractItemModel;
class QSortFilterProxyModel;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void setUserData(int row, const UserData &userData, QStandardItemModel *model);
    UserData getUserData(int row, const QStandardItemModel &model) const;

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void changeEditMode(bool editable); // To mode: editable and uneditable.
    void removeRow();    // Remove the selected row.
    void changeHorizontalHeader(int logicalIndex); // Change the clicked header section's label of table view.
    void updateVerticalHeader();
    bool save(); // Save the table's content.
    void openNotesDialog();
    void search();
    void import();
    void exportHtml();

    void openCreateDialog();
    void openEditDialog(const QModelIndex &index);
    void tableDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void openSetupDialog();

    void copyPointedIndexText();
    void focusSearchLineEdit();

    void resizeHeaderSection(int logicalIndex, int oldSize, int newSize);

private:
    void setupTableView();
    void createActions();
    void createDelegates();
    void createConnections();
    void showAccumulate();

    QString getItemDataString(int row, int column, const QStandardItemModel &model) const;
    void setItemDataString(int row, int column, const QString &str, QStandardItemModel *model);
    QStringList getReservedLabels() const;

    bool writeFile(const QString &fileName); // Write the table's content to file.
    bool readFile(const QString &fileName);  // Read the table's content from file.
    void saveHorizontalHeaderState(QDataStream &out);   // Write the table's horizontal header state to data stream.
    void restoreHorizontalHeaderState(QDataStream &in); // Read the table's horizontal header state from data stream.

    QTableView* getSelectedTableView() const;
    QStandardItemModel* getSelectedModel() const;

    void readSettings();
    void writeSettings();

    QString convertTableToHtml(const QSortFilterProxyModel &model);
    QString getHeaderLabel(int visualIndex, const QHeaderView &headerView, const QAbstractItemModel &model);

    Ui::MainWidget *ui;
    MainWidgetPrivate *data;
};

#endif // MAINWIDGET_H
