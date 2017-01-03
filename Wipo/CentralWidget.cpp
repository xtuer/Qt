#include "CentralWidget.h"
#include "ui_CentralWidget.h"

#include <QButtonGroup>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CentralWidget) {
    ui->setupUi(this);

    QButtonGroup *bg = new QButtonGroup(this);
    bg->addButton(ui->indexButton);
    bg->addButton(ui->analysisAdvanceButton);
    bg->addButton(ui->analysisBaseButton);
    bg->addButton(ui->analysisInfoButton);
    bg->setExclusive(true);

    ui->indexButton->click();
}

CentralWidget::~CentralWidget() {
    delete ui;
}
