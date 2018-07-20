#include "QualityWidget.h"
#include "ui_QualityWidget.h"
#include "util/Json.h"
#include "util/UiUtil.h"
#include "service/Service.h"

#include <QDebug>
#include <QStandardItem>
#include <algorithm>

QualityWidget::QualityWidget(QWidget *parent) : QWidget(parent), ui(new Ui::QualityWidget) {
    initialize();
    handleEvents();
}

QualityWidget::~QualityWidget() {
    delete ui;
    delete qualityJson;
}

// 设置学段和学科
void QualityWidget::setPhaseNameAndSubjectName(const QString &phaseName, const QString &subjectName) {
    this->phaseName = phaseName;
    this->subjectName = subjectName;
}

// 获取选中的认知发展
QString QualityWidget::getSelectedQualities() const {
    QStringList result;
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();

    for (const QModelIndex &index : indexes) {
        result << index.data().toString();
    }

    // 选中多个学业质量水平时按字母序升序排序
    if (result.size() > 1) {
        std::sort(result.begin(), result.end(), [](const QString &a, const QString &b) -> bool {
            return a < b;
        });
    }

    return result.join(",");
}

// 是否选择学业质量水平
bool QualityWidget::isSelected() const {
    return selected;
}

void QualityWidget::showEvent(QShowEvent *event) {
    // 显示的时候设置为还没有选择，并且取消上传选中的单元格
    selected = false;
    ui->tableView->selectionModel()->clearSelection();
    loadQualities(phaseName, subjectName);
    QWidget::showEvent(event);
}

// 初始化
void QualityWidget::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    qualityJson = new Json("data/quality.json", true); // 认知发展的文件

    // 学业质量水平表格
    qualitiesModel = new QStandardItemModel(this);
    qualitiesModel->setHorizontalHeaderLabels({ "学科素养", "学业质量水平", "水平细分", "水平编码" });
    ui->tableView->setModel(qualitiesModel);
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 400);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// 事件处理
void QualityWidget::handleEvents() {
    // 双击单元格
    connect(ui->tableView, &QAbstractItemView::doubleClicked, [this] (const QModelIndex &index) {
        if (index.isValid()) {
            selected = true;
            UiUtil::findWindow(this)->close();
        }
    });

    // 点击确定按钮
    connect(ui->okButton, &QPushButton::clicked, [this] {
        selected = true;
        UiUtil::findWindow(this)->close();
    });

    // 点击取消按钮
    connect(ui->cancelButton, &QPushButton::clicked, [this] {
        UiUtil::findWindow(this)->close();
    });
}

// 加载 [学段 -> 学科] 的学业质量水平
void QualityWidget::loadQualities(const QString &phaseName, const QString &subjectName) {
    qualitiesModel->removeRows(0, qualitiesModel->rowCount()); // 清楚

    QJsonArray array = qualityJson->getJsonArray("."); // 第一级节点是数组
    for (QJsonArray::const_iterator it = array.begin(); it != array.end(); ++it) {
        QJsonObject frog = it->toObject();

        // 找到 [学段 -> 学科] 下的认知发展
        if (qualityJson->getString("phase", "", frog) != phaseName ||
            qualityJson->getString("subject", "", frog) != subjectName) {
            continue;
        }

        QJsonArray qualities = qualityJson->getJsonArray("qualities", frog);
        for (QJsonArray::const_iterator qit = qualities.begin(); qit != qualities.end(); ++qit) {
            QJsonObject quality = qit->toObject();
            QString literacy = qualityJson->getString("name", "", quality); // 学科素养

            QJsonArray levels = qualityJson->getJsonArray("children", quality); // 学业质量水平数组
            for (QJsonArray::const_iterator lit = levels.begin(); lit != levels.end(); ++lit) {
                QJsonObject level = lit->toObject();
                QString levelName = qualityJson->getString("name", "", level); // 学业质量水平: 水平一

                QJsonArray subsectons = qualityJson->getJsonArray("children", level); // 水平细分数组
                for (QJsonArray::const_iterator sit = subsectons.begin(); sit != subsectons.end(); ++sit) {
                    QJsonObject subsection = sit->toObject(); // 水平细分
                    QString subsectionName = qualityJson->getString("name", "", subsection);
                    QString subsectionCode = qualityJson->getString("code", "", subsection);

                    // 0 为学科素养, 1 为学业质量水平, 2 为水平细分, 3 为水平编码
                    QStandardItem *item0 = new QStandardItem(literacy);
                    QStandardItem *item1 = new QStandardItem(levelName);
                    QStandardItem *item2 = new QStandardItem(subsectionName);
                    QStandardItem *item3 = new QStandardItem(subsectionCode);

                    // 1, 2, 3 列不可选择，第 3 列设置提示，第 4 列文本居中
                    item0->setFlags(Qt::NoItemFlags);
                    item1->setFlags(Qt::NoItemFlags);
                    item2->setFlags(Qt::NoItemFlags);
                    item2->setData(subsectionName, Qt::ToolTipRole);
                    item3->setTextAlignment(Qt::AlignCenter);

                    qualitiesModel->appendRow({ item0, item1, item2, item3 });
                }
            }
        }

        // 合并单元格
        mergeCells(0);
        mergeCells(1);

        break;
    }
}

// 合并单元格
void QualityWidget::mergeCells(int col) {
    int rowCount = qualitiesModel->rowCount();

    int row;
    int startRow = 0;
    int rowSpanCount = 1;

    // 如果上下 2 个单元格的内容一样，则 rowSpanCount 加一
    // 如果不一样则合并 rowSpanCount 行单元格
    for (startRow = 0, row = 1; row < rowCount; ++row) {
        QString previous = qualitiesModel->index(row-1, col).data().toString();
        QString current  = qualitiesModel->index(row, col).data().toString();

        if (current == previous) {
            rowSpanCount++;
        } else {
            if (rowSpanCount != 1) {
                ui->tableView->setSpan(startRow, col, rowSpanCount, 1);
            }

            startRow = row;
            rowSpanCount = 1;
        }
    }

    if (rowSpanCount != 1) {
        ui->tableView->setSpan(startRow, col, rowSpanCount, 1);
    }
}

