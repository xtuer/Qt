#include "IntroductionWidget.h"
#include "ui_IntroductionWidget.h"

#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QCursor>

IntroductionWidget::IntroductionWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IntroductionWidget) {
    ui->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);

    // 设置封面动画
    coverMovie = new QMovie(QApplication::applicationDirPath() + "/resources/cover.gif");
    ui->label->setMovie(coverMovie);
    coverMovie->start();

    startButton = new QPushButton("", this);
    startButton->setGeometry(718, 608, 190, 70);
    startButton->setFlat(true);
    startButton->setCursor(Qt::PointingHandCursor);

    connect(coverMovie, &QMovie::frameChanged, [=](int frameNumber) {
        if (frameNumber == coverMovie->frameCount()-1) {
            coverMovie->stop();
        }
    });

    connect(startButton, &QPushButton::clicked, [=] {
        this->hide();
    });
}

IntroductionWidget::~IntroductionWidget() {
    delete ui;
}
