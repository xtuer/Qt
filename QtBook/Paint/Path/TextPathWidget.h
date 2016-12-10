#ifndef TEXTPATHWIDGET_H
#define TEXTPATHWIDGET_H

#include <QWidget>

namespace Ui {
class TextPathWidget;
}

class TextPathWidget : public QWidget {
    Q_OBJECT

public:
    explicit TextPathWidget(QWidget *parent = 0);
    ~TextPathWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::TextPathWidget *ui;
};

#endif // TEXTPATHWIDGET_H
