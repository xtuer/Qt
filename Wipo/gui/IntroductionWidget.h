#ifndef INTRODUCTIONWIDGET_H
#define INTRODUCTIONWIDGET_H

#include <QWidget>

namespace Ui {
class IntroductionWidget;
}

class IntroductionWidget : public QWidget {
    Q_OBJECT

public:
    explicit IntroductionWidget(QWidget *parent = 0);
    ~IntroductionWidget();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::IntroductionWidget *ui;
};

#endif // INTRODUCTIONWIDGET_H
