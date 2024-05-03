#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CentralWidget;
}
QT_END_NAMESPACE

class CentralWidget : public QMainWindow
{
    Q_OBJECT

public:
    CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

private:
    // 生成二维码。
    QPixmap qrEncode(const QString &info);

    Ui::CentralWidget *ui;
};
#endif // CENTRALWIDGET_H
