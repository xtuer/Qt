#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "HttpClient.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    networkAccessManager = new QNetworkAccessManager();
    ui->productIdLineEdit->setValidator(new QIntValidator(this));

    connect(ui->loveSlider, &QSlider::valueChanged, [this] (int value) {
        ui->loveLabel->setText(QString::number(value));
    });

    connect(ui->buySlider, &QSlider::valueChanged, [this] (int value) {
        ui->buyLabel->setText(QString::number(value));
    });

    connect(ui->startButton, &QPushButton::clicked, [this] {
        if (ui->productIdLineEdit->text().trimmed().isEmpty()) {
            ui->plainTextEdit->appendPlainText("请输入产品 ID");
            return;
        }

        increase();
    });
}

MainWidget::~MainWidget() {
    delete ui;
    delete networkAccessManager;
}

void MainWidget::increase() {
    int productId = ui->productIdLineEdit->text().trimmed().toInt();
    int loveCount = ui->loveSlider->value();
    int buyCount = ui->buySlider->value();

    finishedCount = 0;
    totalCount = loveCount + buyCount;

    QString buyUrl = QString("http://i.edu-edu.com.cn/sale/public/comments/counter/add/1/%1/1/jsonp/__sale_product_counter_loaded?_=1474179046754").arg(productId);
    QString loveUrl = QString("http://i.edu-edu.com.cn/sale/public/comments/counter/add/1/%1/2/jsonp/__sale_product_counter_loaded?_=1474179046754").arg(productId);

    // buy
    for (int i = 0; i < buyCount; ++i) {

    }

    // love
    for (int i = 0; i < loveCount; ++i) {
        HttpClient(loveUrl).useManager(networkAccessManager).debug(true).get([this](const QString &response) {
            Q_UNUSED(response)
            qDebug() << response;
//            ++finishedCount;

//            if (finishedCount >= totalCount) {
//                ui->plainTextEdit->appendPlainText("完成");
//            }
        }, [this](const QString &error) {
            ui->plainTextEdit->appendPlainText(error);
            qDebug() << error;
//            ++finishedCount;

//            if (finishedCount >= totalCount) {
//                ui->plainTextEdit->appendPlainText("完成");
//            }
        });
    }
}
