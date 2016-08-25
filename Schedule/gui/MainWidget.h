#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QHash>
#include <QWidget>
#include <QButtonGroup>

class QAbstractButton;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void changeCurrentWidget(QAbstractButton *button);

private:
    void initializeWidgets();
    void createLoadQssAction();
    void createRelatedWidget(QAbstractButton *button);

    Ui::MainWidget *ui;

    QButtonGroup *sidebarButtonGroup;
    QHash<QAbstractButton *, QWidget *> buttonWidgetHash;
};

#endif // MAINWIDGET_H
