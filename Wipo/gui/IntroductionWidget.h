#ifndef INTRODUCTIONWIDGET_H
#define INTRODUCTIONWIDGET_H

#include <QWidget>
class TitleLabel;

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
    TitleLabel *titleLabel;
};

#endif // INTRODUCTIONWIDGET_H
