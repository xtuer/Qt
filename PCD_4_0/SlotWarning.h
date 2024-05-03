#ifndef SLOTWARNING_H
#define SLOTWARNING_H

#include <QWidget>

namespace Ui {
class SlotWarning;
}

/**
 * @brief 报警界面。
 */
class SlotWarning : public QWidget {
    Q_OBJECT

public:
    explicit SlotWarning(QWidget *parent = nullptr);
    ~SlotWarning();

signals:
    void emitBack();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::SlotWarning *ui;
};

#endif // SLOTWARNING_H
