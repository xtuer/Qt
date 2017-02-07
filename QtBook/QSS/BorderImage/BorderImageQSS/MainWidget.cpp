#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "RoundButtonWidget.h"
#include "FillModeWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->roundButton, &QPushButton::clicked, [] {
        (new RoundButtonWidget)->show();
    });

    connect(ui->fillModeButton, &QPushButton::clicked, [] {
        (new FillModeWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
