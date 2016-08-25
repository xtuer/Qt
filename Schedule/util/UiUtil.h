#ifndef UIUTIL_H
#define UIUTIL_H

//#include <QList>
class QString;
class QWidget;
class QStackedWidget;
class ScheduleDescription;
class ScheduleItem;
class QTableView;
class QModelIndex;
template<typename ScheduleItem> class QList;

class UiUtil {
public:
    static void loadQss(); // 为整个应用程序加载 QSS
    static void updateQss(QWidget* widget); // 修改过 widget 的属性后，使其对应的 Style Sheet 生效

    static void addWidgetIntoStackedWidget(QWidget *widget, QStackedWidget *stackedWidget,
                                           bool toLeft = true,
                                           bool toTop = true,
                                           bool toRight = true,
                                           bool toBottom = true);
    // 设置 Widget 的 padding 和 spacing
    static void setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing);

    static QModelIndex getTableViewSelectedIndex(QTableView *view);
    static void appendTableViewRow(QTableView *view, int editColumn);
    static void removeTableViewSelectedRow(QTableView *view);

    // 显示课程表
    static void showSchedule(QWidget *scheduleWidget,
                             const ScheduleDescription &desc,
                             const QList<ScheduleItem> &items,
                             bool classVisible,
                             bool teacherVisible,
                             int classId);

    static void deleteChildrenAndLayout(QWidget *w);
};

#endif // UIUTIL_H
