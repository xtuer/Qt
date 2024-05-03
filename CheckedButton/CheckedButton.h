#ifndef CHECKEDBUTTON_H
#define CHECKEDBUTTON_H

#include <QWidget>
#include <QStyle>

namespace Ui {
class CheckedButton;
}

class CheckedButton : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)

public:
    explicit CheckedButton(QWidget *parent = nullptr);
    ~CheckedButton();

    void setChecked(bool checked) {
        this->checked = checked;

        // Q_PROPERTY 的属性值变化后使得 QSS 生效。
        this->style()->unpolish(this);
        this->style()->polish(this);
    }

    bool isChecked() {
        return this->checked;
    }

signals:
    void clicked(bool checked);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::CheckedButton *ui;
    bool checked;
};

#endif // CHECKEDBUTTON_H
