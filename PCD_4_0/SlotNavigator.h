#ifndef SLOTNAVIGATOR_H
#define SLOTNAVIGATOR_H

#include <QWidget>

class SlotHome;
class SlotParams;
class SlotWarning;
class SlotHistory;

namespace Ui {
class SlotNavigator;
}

/**
 * @brief 每个 slot 的导航界面。
 */
class SlotNavigator : public QWidget {
    Q_OBJECT

public:
    explicit SlotNavigator(QWidget *parent = nullptr);
    ~SlotNavigator();

private:
    void initUi();
    void handleEvents();

    Ui::SlotNavigator *ui;

    SlotHome *slotHome;
    SlotParams *slotParams;
    SlotWarning *slotWarning;
    SlotHistory *slotHistory;
};

#endif // SLOTNAVIGATOR_H
