#include "MainWidget.h"
#include "ui_MainWidget.h"

#include "DrawSimpleTextWidget.h"
#include "FontMetricsWidget.h"
#include "CentralTextWidget.h"
#include "WrapTextWidget.h"
#include "AutoResizeTextWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->drawSimpleTextButton, &QPushButton::clicked, [] {
        (new DrawSimpleTextWidget)->show();
    });

    connect(ui->fontMetricsButton, &QPushButton::clicked, [] {
        (new FontMetricsWidget)->show();
    });

    connect(ui->centralTextButton, &QPushButton::clicked, [] {
        (new CentralTextWidget)->show();
    });

    connect(ui->wrapTextButton, &QPushButton::clicked, [] {
        (new WrapTextWidget)->show();
    });

    connect(ui->autoResizeTextButton, &QPushButton::clicked, [] {
        (new AutoResizeTextWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
