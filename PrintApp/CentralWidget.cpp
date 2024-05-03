#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "QR_Encode.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CentralWidget) {
    ui->setupUi(this);

    // 去掉不需要的 Label 上的文字 (Designer 上设置文字是为了设计时方便看到)。
    ui->qrLabel->setText("");
    ui->brandLabel->setText("");

    // 生成并显示二维码图片。
    QString rechargeUri = "/api/increase/12349969646/test";
    QPixmap qrPixmap = qrEncode(rechargeUri);
    ui->qrLabel->setPixmap(qrPixmap.scaled(ui->qrLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

CentralWidget::~CentralWidget() {
    delete ui;
}

// 生成二维码。
// 注意: 使用 https://github.com/abinghu/qt_rc/tree/master 生成二维码，好像进度不高，不支持中文，优点是使用简单。
//      如果要使用更好的二维码生成库，可以继续研究。
QPixmap CentralWidget::qrEncode(const QString &info) {
    int  levelIndex   = 2;
    int  versionIndex = 0;
    bool extented     = true;
    int  maskIndex    = 4;

    CQR_Encode qrEncode;
    qrEncode.EncodeData(levelIndex, versionIndex, extented, maskIndex, info.toUtf8().data());

    // 创建二维码图片
    int encodeImageSize = qrEncode.m_nSymbleSize + (QR_MARGIN * 2);
    QImage encodeImage(encodeImageSize, encodeImageSize, QImage::Format_Mono);
    encodeImage.fill(1);

    // 生成二维码图片
    for (int i=0; i<qrEncode.m_nSymbleSize; ++i) {
        for (int j=0; j<qrEncode.m_nSymbleSize; ++j) {
            if (qrEncode.m_byModuleData[i][j])
                encodeImage.setPixel(i+QR_MARGIN, j+QR_MARGIN, 0);
        }
    }

    return QPixmap::fromImage(encodeImage);
}
