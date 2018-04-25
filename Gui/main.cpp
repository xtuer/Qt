#include "RegionModel.h"

#include <QApplication>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>
#include <QList>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // key 为地区的名字 (第一级例外)，value 为此地区的下级地区名字的 list
    QMap<QString, QList<QString> > regions;
    regions["所有省"] = QList<QString>() << "山东省" << "江苏省";
    regions["山东省"] = QList<QString>() << "济南市" << "青岛市" << "淄博市";
    regions["江苏省"] = QList<QString>() << "南京市" << "无锡市" << "徐州市";
    regions["淄博市"] = QList<QString>() << "即墨市" << "城阳区";

    QStandardItemModel *model = new RegionModel();
    model->setHorizontalHeaderLabels(QStringList() << "省市县" << "序号");

    // 遍历所有的省，创建省的节点
    int n = 0;
    for (QString province : regions.value("所有省")) {
        QStandardItem *provinceItem = new QStandardItem(province);
        model->appendRow(QList<QStandardItem*>() << provinceItem << new QStandardItem(QString::number(++n))); // [*] 创建了多列

        // 遍历当前省下的市，创建市的节点
        for (QString city : regions.value(province)) {
            QStandardItem *cityItem = new QStandardItem(city);
            provinceItem->appendRow(cityItem); // [*] 只创建了一列

            // 遍历当前市下的县，创建县的节点
            for (QString county : regions.value(city)) {
                QStandardItem *countyItem = new QStandardItem(county);
                cityItem->appendRow(QList<QStandardItem*>() << countyItem << new QStandardItem(QString::number(++n))); // [*] 创建了多列
            }
        }
    }

    QTreeView *view = new QTreeView();
    view->setModel(model);
    view->resize(500, 500);
    view->setColumnWidth(0, 150);
    view->show();
    view->expandAll();

    view->setDragEnabled(true);
    view->setAcceptDrops(true);
    view->setDropIndicatorShown(true);
    view->setDragDropMode(QAbstractItemView::InternalMove);

    return a.exec();
}
