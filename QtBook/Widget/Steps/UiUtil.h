#ifndef UIUTIL_H
#define UIUTIL_H

class QWidget;

class UiUtil {
public:
    static void loadQss(); // 为整个应用程序加载 QSS
    static void updateQss(QWidget* widget); // 修改过 widget 的属性后，使此属性对应的 Style Sheet 生效
};

#endif // UIUTIL_H
