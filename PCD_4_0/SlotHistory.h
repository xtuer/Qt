#ifndef SLOTHISTORY_H
#define SLOTHISTORY_H

#include <QWidget>

class QStandardItemModel;

namespace Ui {
class SlotHistory;
}

/**
 * @brief 历史结果界面。
 */
class SlotHistory : public QWidget {
    Q_OBJECT

public:
    explicit SlotHistory(QWidget *parent = nullptr);
    ~SlotHistory();

signals:
    void emitBack();

private:
    void initUi();
    void handleEvents();

    /**
     * @brief 获取第 pageNumber 页的数据 (最多 5 行、固定 6 列)。
     * @param pageNumber 页码。
     * @return 返回指定页的数据。
     */
    QList<QList<QString> > getPageData(int pageNumber);

    /**
     * @brief 设置第 pageNumber 页的数据到 table view 里。
     * @param pageNumber 页码。
     */
    void populatePageData(int pageNumber);

    Ui::SlotHistory *ui;
    QStandardItemModel *model;
    int pageNumber { 1 }; // 页码，默认为第 1 页。
};

#endif // SLOTHISTORY_H
