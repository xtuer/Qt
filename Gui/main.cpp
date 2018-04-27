#include <QApplication>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>
#include <QList>
#include <QAction>
#include <QMenu>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // key 为地区的名字 (第一级例外)，value 为此地区的下级地区名字的 list
    QMap<QString, QList<QString> > regions;
    regions["所有省"] = QList<QString>() << "山东省" << "江苏省";
    regions["山东省"] = QList<QString>() << "济南市" << "青岛市" << "淄博市";
    regions["江苏省"] = QList<QString>() << "南京市" << "无锡市" << "徐州市";
    regions["淄博市"] = QList<QString>() << "即墨市" << "城阳区";

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << "省市县" << "序号");

    // 遍历所有的省，创建省的节点
    int n = 0;
    for (QString province : regions.value("所有省")) {
        QStandardItem *provinceItem = new QStandardItem(province);
        model->appendRow(provinceItem); // [*] 创建了多列

        // 遍历当前省下的市，创建市的节点
        for (QString city : regions.value(province)) {
            QStandardItem *cityItem = new QStandardItem(city);
            provinceItem->appendRow(cityItem); // [*] 只创建了一列

            // 遍历当前市下的县，创建县的节点
            for (QString county : regions.value(city)) {
                QStandardItem *countyItem = new QStandardItem(county);
                cityItem->appendRow(countyItem); // [*] 创建了多列
            }
        }
    }

    QTreeView *view = new QTreeView();
    view->setModel(model);
    view->resize(500, 500);
    view->setColumnWidth(0, 150);
    view->show();
    view->expandAll();

    view->setContextMenuPolicy(Qt::CustomContextMenu); // 启用右键菜单

    QAction *createAction = new QAction("创建", view);
    QAction *deleteAction = new QAction("删除", view);

    QObject::connect(view, &QTreeView::customContextMenuRequested, [=] {
        QMenu menu;
        QPoint posAtViewport = view->viewport()->mapFromGlobal(QCursor::pos());
        QModelIndex index = view->indexAt(posAtViewport); // 右键菜单点击的节点

        menu.addAction(createAction);

        // 根据节点动态显示菜单项，这也是为什么右键菜单在英文里叫 context menu 的原因，因为根据上下文来决定要显示的菜单项
        if (index.isValid()) {
            menu.addAction(deleteAction);
        }

        menu.exec(QCursor::pos());
    });

    return a.exec();
}
