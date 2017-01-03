#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class Json;
class QModelIndex;
class QAbstractButton;
class CentralWidgetPrivate;

namespace Ui {
class CentralWidget;
}

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

private slots:
    void showCourses(QAbstractButton *button);      // 显示模块对应的课程
    void showCoursewares(const QModelIndex &index); // 显示课程对应的课件
    void openVideo(const QModelIndex &index);       // 打开课件的视频

private:
    void createLoadQssAction(); // 创建加载 QSS 的 Action，这样就能使用快捷键加载 QSS，方便调试

    Ui::CentralWidget *ui;
    CentralWidgetPrivate *d;
};

#endif // CENTRALWIDGET_H
