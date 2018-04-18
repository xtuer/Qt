#ifndef MESSAGESHOWER_H
#define MESSAGESHOWER_H

#include <QWidget>

class QLabel;

namespace Ui {
class MessageShower;
}

class MessageShower : public QWidget {
    Q_OBJECT
public:
    explicit MessageShower(QWidget *parent = nullptr);

public slots:
    void showMessage(const QString &text);

private:
    Ui::MessageShower *ui;
    QLabel *lastMessage = NULL;
};

#endif // MESSAGESHOWER_H
