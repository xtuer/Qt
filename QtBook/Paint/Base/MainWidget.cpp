#include "MainWidget.h"
#include "ui_MainWidget.h"

#include "GridWidget.h"
#include "MultipleLinesWidget.h"
#include "PolylineWidget.h"
#include "PolygonWidget.h"
#include "PolygonCircleWidget.h"
#include "RoundRectWidget.h"
#include "EllipseWidget.h"
#include "ArcChordPieWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->gridButton, &QPushButton::clicked, [] {
        (new GridWidget)->show();
    });

    connect(ui->multipleLinesButton, &QPushButton::clicked, [] {
        (new MultipleLinesWidget)->show();
    });

    connect(ui->polylineButton, &QPushButton::clicked, [] {
        (new PolylineWidget)->show();
    });

    connect(ui->polygonButton, &QPushButton::clicked, [] {
        (new PolygonWidget)->show();
    });

    connect(ui->polygonCircleButton, &QPushButton::clicked, [] {
        (new PolygonCircleWidget)->show();
    });

    connect(ui->roundRectButton, &QPushButton::clicked, [] {
        (new RoundRectWidget)->show();
    });

    connect(ui->ellipseButton, &QPushButton::clicked, [] {
        (new EllipseWidget)->show();
    });

    connect(ui->arcChordPieButton, &QPushButton::clicked, [] {
        (new ArcChordPieWidget)->show();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
