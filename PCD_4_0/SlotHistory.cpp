#include "SlotHistory.h"
#include "ui_SlotHistory.h"

#include <QStandardItemModel>

const int ROW = 5;
const int COL = 6;

SlotHistory::SlotHistory(QWidget *parent): QWidget(parent), ui(new Ui::SlotHistory) {
    initUi();
    handleEvents();

    // 初始化显示第 1 页的数据。
    populatePageData(pageNumber);
}

SlotHistory::~SlotHistory() {
    delete ui;
}

void SlotHistory::initUi() {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);

    ui->tableView->verticalHeader()->hide();      // 隐藏垂直表头。
    ui->tableView->setAlternatingRowColors(true); // 奇偶行不同颜色。
    ui->tableView->setShowGrid(false);            // 隐藏网格线。
    ui->tableView->verticalHeader()->setDefaultSectionSize(56);        // 设置默认行高 (不能用 QSS 设置)。
    ui->tableView->horizontalHeader()->setDefaultSectionSize(154);     // 设置默认列宽 (不能用 QSS 设置)。
    // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);    // 最后一列占据剩下空间。
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不让编辑。

    // 固定 5 行 6 列。
    model = new QStandardItemModel(5, 6);
    model->setHorizontalHeaderLabels(
        QStringList() << "日期时间" << "灭菌器号" << "灭菌锅次" << "灭菌温度" << "维持时间" << "检测结果"
    );

    // 初始化 5 行 6 列空数据。
    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            QStandardItem *item = new QStandardItem("");
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(row, col, item);
        }
    }

    ui->tableView->setModel(model);
}

void SlotHistory::handleEvents() {
    connect(ui->backButton, &QPushButton::clicked, this, &SlotHistory::emitBack);

    // 加载显示下一页的数据。
    connect(ui->next1PageButton, &QPushButton::clicked, this, [this] {
        pageNumber++;
        populatePageData(pageNumber);
    });

    // 加载显示上一页的数据。
    connect(ui->prev1PageButton, &QPushButton::clicked, this, [this] {
        if (pageNumber <= 1) {
            return;
        }

        pageNumber--;
        populatePageData(pageNumber);
    });
}

// 获取第 pageNumber 页的数据 (最多 5 行、固定 6 列)。
QList<QList<QString> > SlotHistory::getPageData(int pageNumber) {
    QList<QList<QString> > pageRows;

    for (int row = 0; row < ROW; ++row) {
        QList<QString> rowData;

        for (int col = 0; col < COL; ++col) {
            QString text = QString("Row:%1, Col:%2").arg(pageNumber * row + 1).arg(col + 1);
            rowData << text;
        }

        pageRows << rowData;
    }

    return pageRows;
}

// 设置第 pageNumber 页的数据到 table view 里。
void SlotHistory::populatePageData(int pageNumber) {
    /*
     * 逻辑:
     * 1. 获取第 pageNumber 页的数据。
     * 2. 清空表格中的数据。
     * 3. 设置表格的数据。
     */

    // [1] 获取第 pageNumber 页的数据。
    QList<QList<QString> > pageRows = getPageData(pageNumber);

    // [2] 清空表格中的数据。
    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            QModelIndex index = model->index(row, col);
            model->setData(index, "");
        }
    }

    // [3] 设置表格的数据。
    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            QString text = pageRows.at(row).at(col);
            QModelIndex index = model->index(row, col);
            model->setData(index, text);
        }
    }
}
