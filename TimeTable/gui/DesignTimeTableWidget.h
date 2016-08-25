#ifndef DESIGNTABLEWIDGET_H
#define DESIGNTABLEWIDGET_H

#include <QWidget>
#include <QList>

class TimeTableData;
class QLabel;
class QButtonGroup;
class QAbstractButton;

namespace Ui {
class DesignTimeTableWidget;
}

class DesignTimeTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit DesignTimeTableWidget(QWidget *parent = 0);
    ~DesignTimeTableWidget();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void updateTimeTableData(QAbstractButton *button);
    void readTimeTableData();
    void writeTimeTableData();

private:
    void editTitle(int x, int y);
    Ui::DesignTimeTableWidget *ui;
    QButtonGroup *buttonGroup;
    TimeTableData *timeTableData;
};

#endif // DESIGNTABLEWIDGET_H
