#ifndef INTRODUCTIONWIDGET_H
#define INTRODUCTIONWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QPushButton>

namespace Ui {
class IntroductionWidget;
}

class IntroductionWidget : public QWidget {
    Q_OBJECT

public:
    explicit IntroductionWidget(QWidget *parent = 0);
    ~IntroductionWidget();

private:
    Ui::IntroductionWidget *ui;
    QMovie *coverMovie;
    QPushButton *startButton;
};

#endif // INTRODUCTIONWIDGET_H
