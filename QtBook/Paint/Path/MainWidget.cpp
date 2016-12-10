#include "MainWidget.h"
#include "ui_MainWidget.h"

#include "ComplicatedPathWidget.h"
#include "TextPathWidget.h"
#include "AnimationAlongPathWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->complicatedPathButton, &QPushButton::clicked, [] {
        (new ComplicatedPathWidget)->show();
    });

    connect(ui->textPathButton, &QPushButton::clicked, [] {
        (new TextPathWidget)->show();
    });

    connect(ui->animationAlongPathButton, &QPushButton::clicked, [] {
        (new AnimationAlongPathWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
