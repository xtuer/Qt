#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QHash>

class QAbstractButton;

namespace Ui {
class CentralWidget;
}

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

private slots:
    void changeCurrentWidget(QAbstractButton *button);

private:
    void initialize();
    void createLoadQssAction();
    void createRelatedWidget(QAbstractButton *button);

    Ui::CentralWidget *ui;
    QHash<QAbstractButton *, QWidget *> buttonWidgetHash; // key 是左侧按钮指针，value 是右侧 widget 的指针
};

#endif // CENTRALWIDGET_H
