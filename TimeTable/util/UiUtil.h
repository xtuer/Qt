#ifndef UIUTIL_H
#define UIUTIL_H

class QWidget;
class QTableView;
class QModelIndex;
class TimeTableData;
class TimeTableItem;
class QStackedWidget;
class QString;
class QSplitter;
template <typename T> class QList;

class UiUtil {
public:
    static void loadStyleSheet();
    static void updateStyleSheet(QWidget *widget);

    static QModelIndex getTableViewSelectedIndex(QTableView *view);
    static void removeTableViewSelectedRow(QTableView *view);
    static void appendTableViewRow(QTableView *view, int editColumn = 0);

    static void addWidgetIntoStackedWidget(QWidget *widget, QStackedWidget *stackedWidget,
                                           bool toLeft = true,
                                           bool toTop = true,
                                           bool toRight = true,
                                           bool toBottom = true);

    static void updateTimeTable(QWidget *timeTableWidget,   const TimeTableData &timeTableData,
                                bool showClazz = true,
                                bool showTeacher = true);
    static void setTimeTableItems(QWidget *timeTableWidget, const QList<TimeTableItem*> &items);
    static void getTimeTableItems(QWidget *timeTableWidget, QList<TimeTableItem*> *items);
};

#endif // UIUTIL_H
