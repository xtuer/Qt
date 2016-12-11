#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "LinearGradientWidget.h"
#include "LinearGradientAlgorithmWidget.h"
#include "RadialGradientWidget.h"
#include "ConicalGradientWidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->linearGradientButton, &QPushButton::clicked, [] {
        (new LinearGradientWidget)->show();
    });

    connect(ui->linearGradientAlgorithmButton, &QPushButton::clicked, [] {
        (new LinearGradientAlgorithmWidget)->show();
    });

    connect(ui->radialGradientButton, &QPushButton::clicked, [] {
        (new RadialGradientWidget)->show();
    });

    connect(ui->conicalGradientButton, &QPushButton::clicked, [] {
        (new ConicalGradientWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
