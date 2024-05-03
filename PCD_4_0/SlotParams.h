#ifndef SLOTPARAMS_H
#define SLOTPARAMS_H

#include <QWidget>

namespace Ui {
class SlotParams;
}

/**
 * @brief 灭菌参数界面。
 */
class SlotParams : public QWidget {
    Q_OBJECT

public:
    explicit SlotParams(QWidget *parent = nullptr);
    ~SlotParams();

    /**
     * @brief 设置灭菌参数，显示到界面上。
     */
    void setParams(const QString param);

signals:
    void emitBack();

private:
    void initUi();
    void handleEvents();

    Ui::SlotParams *ui;
    QString param;
};

#endif // SLOTPARAMS_H
