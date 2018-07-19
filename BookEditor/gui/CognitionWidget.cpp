#include "CognitionWidget.h"
#include "ui_CognitionWidget.h"
#include "util/Json.h"
#include "util/UiUtil.h"
#include "service/Service.h"
#include "service/KpService.h"

#include <QDebug>
#include <QStandardItem>

CognitionWidget::CognitionWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CognitionWidget) {
    initialize();
    handleEvents();
}

CognitionWidget::~CognitionWidget() {
    delete ui;
    delete kpService;
    delete cognitionJson;
}

// 设置学段和学科
void CognitionWidget::setPhaseNameAndSubjectName(const QString &phaseName, const QString &subjectName) {
    this->phaseName = phaseName;
    this->subjectName = subjectName;
}

// 获取选中的认知发展
QString CognitionWidget::getSelectedCognitions() const {
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();
    QStringList result;

    for (const QModelIndex &index : indexes) {
        result << index.data().toString();
    }

    return result.join(",");
}

// 是否选择认知发展水平
bool CognitionWidget::isSelected() const {
    return selected;
}

void CognitionWidget::showEvent(QShowEvent *event) {
    // 显示的时候设置为还没有选择，并且取消上传选中的单元格
    selected = false;
    ui->tableView->selectionModel()->clearSelection();
    loadCognitions(phaseName, subjectName);
    QWidget::showEvent(event);
}

// 初始化
void CognitionWidget::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    cognitionJson = new Json("data/cognition.json", true); // 认知发展的文件

    // 认知水平表格
    cognitionsModel = new QStandardItemModel(this);
    cognitionsModel->setHorizontalHeaderLabels({ "认知水平", "含义", "等级" });
    ui->tableView->setModel(cognitionsModel);
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 400);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// 事件处理
void CognitionWidget::handleEvents() {
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

// 加载 [学段 -> 学科] 的认知发展水平
void CognitionWidget::loadCognitions(const QString &phaseName, const QString &subjectName) {
    cognitionsModel->removeRows(0, cognitionsModel->rowCount()); // 清楚
    QJsonArray array = cognitionJson->getJsonArray("."); // 第一级节点是数组

    for (QJsonArray::const_iterator it = array.begin(); it != array.end(); ++it) {
        QJsonObject frog = it->toObject();

        // 找到 [学段 -> 学科] 下的认知发展
        if (cognitionJson->getString("phase", "", frog) != phaseName ||
            cognitionJson->getString("subject", "", frog) != subjectName) {
            continue;
        }

        QJsonArray cognitions = cognitionJson->getJsonArray("cognitions", frog);
        for (QJsonArray::const_iterator cit = cognitions.begin(); cit != cognitions.end(); ++cit) {
            QJsonArray cognition = cit->toArray();

            // 0 为认知水平, 1 为含义，2 为水平
            QStandardItem *item0 = new QStandardItem(cognition.at(0).toString());
            QStandardItem *item1 = new QStandardItem(cognition.at(1).toString());
            QStandardItem *item2 = new QStandardItem(cognition.at(2).toString());

            // 1, 2 列不可选择，第 3 列设置提示，第 4 列文本居中
            item0->setFlags(Qt::NoItemFlags);
            item1->setFlags(Qt::NoItemFlags);
            item1->setData(cognition.at(1).toString(), Qt::ToolTipRole);
            item2->setTextAlignment(Qt::AlignCenter);

            cognitionsModel->appendRow({ item0, item1, item2 });
        }

        break;
    }
}

