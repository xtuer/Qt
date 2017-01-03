#ifndef UIUTIL_H
#define UIUTIL_H

class QString;
class QWidget;
class QStackedWidget;

class UiUtil {
public:
    static void loadQss(); // 为整个应用程序加载 QSS
    static void updateQss(QWidget* widget); // 修改过 widget 的属性后，使其对应的 Style Sheet 生效

    static void addWidgetIntoStackedWidget(QWidget *widget, QStackedWidget *stackedWidget,
                                           bool toLeft = true,
                                           bool toTop = true,
                                           bool toRight = true,
                                           bool toBottom = true);
};

#endif // UIUTIL_H
