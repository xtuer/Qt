#include "ColourfulDevicePlazza.h"
#include "ui_ColourfulDevicePlazza.h"

#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QRandomGenerator>

/*-----------------------------------------------------------------------------|
 |                               ColourfulCircle                               |
 |----------------------------------------------------------------------------*/
class ColourfulCircle : public QLabel {
public:
    ColourfulCircle(const QString &name, QWidget *parent = nullptr);
    ~ColourfulCircle() override;

    void setBackground(const QColor &bgcolor, bool stripe);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor bgcolor = Qt::transparent;
    bool   stripe  = false; // 是否使用条形背景
};

ColourfulCircle::ColourfulCircle(const QString &name, QWidget *parent) : QLabel(name, parent) {
    setObjectName(name);
    setAttribute(Qt::WA_StyledBackground, true);
    setProperty("class", "RoundLabel");
}

ColourfulCircle::~ColourfulCircle() {

}

void ColourfulCircle::setBackground(const QColor &bgcolor, bool stripe) {
    this->bgcolor = bgcolor;
    this->stripe  = stripe;
    this->update();
}

void ColourfulCircle::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(QBrush(bgcolor, stripe ? Qt::DiagCrossPattern : Qt::SolidPattern));

    int w = width();
    int h = height();
    int s = w < h ? w : h;

    painter.translate(w / 2, h / 2);
    painter.setPen(Qt::gray);
    painter.drawRoundedRect(-s/2+1, -s/2+1, s-2, s-2, s, s);
}

/*-----------------------------------------------------------------------------|
 |                            ColourfulDevicePlazza                            |
 |----------------------------------------------------------------------------*/
ColourfulDevicePlazza::ColourfulDevicePlazza(QWidget *parent) : QWidget(parent), ui(new Ui::ColourfulDevicePlazza) {
    initialize();
}

ColourfulDevicePlazza::~ColourfulDevicePlazza() {
    delete ui;
}

void ColourfulDevicePlazza::setDeviceBackground(const QString &deviceName, const QColor &bgcolor, bool stripe) {
    ColourfulCircle *device = findChild<ColourfulCircle *>(deviceName);

    if (nullptr != device) {
        device->setBackground(bgcolor, stripe);
    }
}

void ColourfulDevicePlazza::setPositiveCoordinates(const QString &text) {
    ui->positiveLabel->setText(text);
}

void ColourfulDevicePlazza::setNegativeCoordinates(const QString &text) {
    ui->negativeLabel->setText(text);
}

void ColourfulDevicePlazza::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(R"(
                  #devicesWidget { background: white; border: 1px solid #ddd; border-radius: 4px; }
                  #devicesWidget QLabel { qproperty-alignment: AlignCenter }
                  #devicesWidget .RoundLabel {
                      min-width : 40px;
                      min-height: 40px;
                  }
    )");

    createDevices();
}

void ColourfulDevicePlazza::createDevices() {
    QGridLayout *gl = qobject_cast<QGridLayout *>(ui->devicesWidget->layout());

    int rows = 8;
    int cols = 12;

    // 水平 label
    for (int col = 1; col <= cols; ++col) {
        gl->addWidget(new QLabel(QString::number(col)), 0, col);
    }

    // 垂直 label
    for (int row = 1; row <= rows; ++row) {
        gl->addWidget(new QLabel(QString(char('A'+row-1))), row, 0);
    }

    // 中间有颜色的圆
    for (int row = 1; row <= rows; ++row) {
        for (int col = 1; col <= cols; ++col) {
            // 提示: 设备的名字即是它的坐标，如 A1, D23, E3
            QString name = QString("%1%2").arg(char('A'+row-1)).arg(col);
            ColourfulCircle *label = new ColourfulCircle(name);

            gl->addWidget(label, row, col);
        }
    }
}
