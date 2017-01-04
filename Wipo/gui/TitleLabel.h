#ifndef TITLELABEL_H
#define TITLELABEL_H

#include <QWidget>

class TitleLabel : public QWidget {
    Q_OBJECT
public:
    explicit TitleLabel(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap background;
};

#endif // TITLELABEL_H
