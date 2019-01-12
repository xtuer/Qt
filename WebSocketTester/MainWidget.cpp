#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "Json.h"

#include <QWebSocket>
#include <QUuid>
#include <QTimer>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    // 点击开始按钮, 连接服务器
    connect(ui->startButton, &QPushButton::clicked, [this] {
        ui->startButton->setDisabled(true);
        QString server = ui->urlLineEdit->text().trimmed();
        int amount = ui->amountSpinBox->value();

        // 创建监听服务器所有连接数 Socket
        {
            QWebSocket *socket = new QWebSocket();
            QTimer *timer = new QTimer();

            // 连接成功
            connect(socket, &QWebSocket::connected, [=] {
                timer->start(2000); // 启动定时器, 开始发送请求服务器的连接数
            });

            // 收到连接数消息
            connect(socket, &QWebSocket::textMessageReceived, [=] (const QString &message) {
                Json json(message);
                int count = json.getString("content").toInt();
                ui->allConnectedCountLabel->setText(QString("服务器所有连接: %1").arg(count));
            });

            // 定时发送心跳
            connect(timer, &QTimer::timeout, [=] {
                socket->sendTextMessage("{\"type\": \"CONNECTION_COUNT\"}");
            });

            socket->open(QUrl(createUrl(server, 0)));
        }

        // 创建 amount 个连接
        for (int i = 1; i <= amount; ++i) {
            QWebSocket *socket = new QWebSocket();
            QTimer *timer = new QTimer();

            // 连接成功
            connect(socket, &QWebSocket::connected, [=] {
                connectedCount++;
                ui->connectedCountLabel->setText(QString("已建立连接: %1").arg(connectedCount));
                timer->start(5000); // 启动定时器, 开始发送心跳
            });
            // 连接断开
            connect(socket, &QWebSocket::disconnected, [this] {
                connectedCount--;
                ui->connectedCountLabel->setText(QString("已建立连接: %1").arg(connectedCount));

                if (connectedCount == 0) {
                    ui->startButton->setEnabled(true);
                }
            });
            // 定时发送心跳
            connect(timer, &QTimer::timeout, [=] {
                socket->sendTextMessage("{\"type\": \"HEARTBEAT\"}");
            });

            socket->open(QUrl(createUrl(server, i)));
        }
    });
}

MainWidget::~MainWidget() {
    delete ui;
}

// 创建 WebSocket 使用的 URL
QString MainWidget::createUrl(const QString &server, int sn) const {
    QString uuid = QUuid::createUuid().toString().toUpper();
    QString url  = QString("%1?userId=%2&username=StressTester_%3").arg(server).arg(uuid).arg(sn);
    url.replace("{", "").replace("}", "").replace("-", "");

    return url;
}
