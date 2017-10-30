#ifndef READINGWIDGET_H
#define READINGWIDGET_H

#include <QWidget>

class ReadingThread;

namespace Ui {
class ReadingWidget;
}

class ReadingWidget : public QWidget {
    Q_OBJECT

public:
    explicit ReadingWidget(QWidget *parent = 0);
    ~ReadingWidget();

private:
    Ui::ReadingWidget *ui;
    ReadingThread *readingThread;
};

#endif // READINGWIDGET_H
