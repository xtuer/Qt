#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "CodeStatusWidget.h"
#include "ApiStatusWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->codeStatusButton, &QPushButton::clicked, [] {
        (new CodeStatusWidget)->show();
    });

    connect(ui->apiStatusButton, &QPushButton::clicked, [] {
        (new ApiStatusWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
