#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    QList<QString> getSuffixes(); // 取得文件的后缀名
    void processBom(const QString &dir, const QList<QString> suffixes, bool needBom); // 添加或者删除文件的 BOM

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
