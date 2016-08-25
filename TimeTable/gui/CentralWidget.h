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
    void changeCurrentWidget(QAbstractButton *);
    void loadStyleSheet();

private:
    void initialize();
    void createLoadStyleAction();
    void readSettings();
    void writeSettings();
    void createRelatedWidget(QAbstractButton *button);

    Ui::CentralWidget *ui;
    QHash<QAbstractButton *, QWidget *> buttonWidgetHash;
};

#endif // CENTRALWIDGET_H
