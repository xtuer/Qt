#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void showChapters(const QString &courseName);
    void openVideo(const QModelIndex &index);

private:
    void readConfig();
    void showCourses();

private:
    Ui::MainWidget *ui;
    QJsonArray courses;
};

#endif // MAINWIDGET_H
