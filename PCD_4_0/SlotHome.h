#ifndef SLOTHOME_H
#define SLOTHOME_H

#include <QWidget>

namespace Ui {
class SlotHome;
}

/**
 * @brief 设计里的 0 号界面。
 */
class SlotHome : public QWidget {
    Q_OBJECT

public:
    explicit SlotHome(QWidget *parent = nullptr);
    ~SlotHome();

signals:
    /**
     * @brief 显示编辑灭菌参数界面。
     */
    void emitEditSterilizationParams();

    /**
     * @brief 显示查看本次结果。
     */
    void emitCurrentResult();

    /**
     * @brief 显示查看历史结果。
     */
    void emitHistoryResult();

private:
    void handleEvents();

    Ui::SlotHome *ui;
};

#endif // SLOTHOME_H
