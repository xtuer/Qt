#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    dumpStructure(ui->calendarWidget, 0);
}

MainWidget::~MainWidget() {
}

/**
 * @brief MainWidget::dumpStructure
 * @param obj
 * @param spaceCount
 */
void MainWidget::dumpStructure(const QObject *obj, int spaceCount) {
    qDebug() << QString("%1%2 : %3")
                .arg("", spaceCount)
                .arg(obj->metaObject()->className())
                .arg(obj->objectName());

    QObjectList list = obj->children();

    foreach (QObject * child, list) {
        dumpStructure(child, spaceCount + 4);
    }
}
