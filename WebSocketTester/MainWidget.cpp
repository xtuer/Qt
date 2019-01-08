#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QWebSocket>
#include <QUuid>
#include <QTimer>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, [this] {
        ui->startButton->setDisabled(true);
        QString url = ui->urlLineEdit->text().trimmed();
        int amount = ui->amountSpinBox->value();

        for (int i = 1; i <= amount; ++i) {
            QString uuid = QUuid::createUuid().toString().toUpper();

            QString wurl = QString("%1?userId=%2&username=StressTester_%3").arg(url).arg(uuid).arg(i);
            wurl.replace("{", "").replace("}", "").replace("-", "");
            qDebug() << wurl;

            QWebSocket *socket = new QWebSocket();
            QTimer *timer = new QTimer();

            connect(socket, &QWebSocket::connected, [=] {
                connectedCount++;
                ui->connectedCountLabel->setText(QString("已建立连接: %1").arg(connectedCount));
                timer->start(5000);
            });
            connect(socket, &QWebSocket::disconnected, [this] {
                connectedCount--;
                ui->connectedCountLabel->setText(QString("已建立连接: %1").arg(connectedCount));

                if (connectedCount == 0) {
                    ui->startButton->setEnabled(true);
                }
            });
            // 发送心跳
            connect(timer, &QTimer::timeout, [=] {
                socket->sendTextMessage("{\"type\": \"HEARTBEAT\"}");
            });

            socket->open(QUrl(wurl));
        }
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
