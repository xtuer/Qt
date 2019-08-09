#include "MyWidget.h"
#include "ui_MyWidget.h"
#include "MessageShower.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QCloseEvent>

#include <QThread>
#include <QTimer>
#include <QItemSelectionModel>

#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QRandomGenerator>

class ColourfulCircle : public QLabel {
public:
    ColourfulCircle(const QString &name, const QString &bgcolor, bool stripe, QWidget *parent = nullptr);
    ~ColourfulCircle() override;

    void setBgcolor(const QColor &bgcolor);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor bgcolor;
    bool   stripe = false; // 是否使用条形背景
};

ColourfulCircle::ColourfulCircle(const QString &name, const QString &bgcolor, bool stripe, QWidget *parent)
    : QLabel(name, parent), bgcolor(QColor(bgcolor)), stripe(stripe){
    setObjectName(name);
    setAttribute(Qt::WA_StyledBackground, true);
    setProperty("class", "RoundLabel");
    setMinimumSize(40, 40);
}

ColourfulCircle::~ColourfulCircle() {

}

void ColourfulCircle::setBgcolor(const QColor &bgcolor) {
    this->bgcolor = bgcolor;
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

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    initializeUi();
    handleEvents();


    QGridLayout *gl = qobject_cast<QGridLayout *>(layout());

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
            int r = QRandomGenerator::global()->bounded(0, 256);
            int g = QRandomGenerator::global()->bounded(0, 256);
            int b = QRandomGenerator::global()->bounded(0, 256);

            bool  stripe = QRandomGenerator::global()->generate() % 5 == 0;
            QString name = QString("%1%2").arg(char('A'+row-1)).arg(col); // 名字和坐标一样: A1, B3 等
            ColourfulCircle *label = new ColourfulCircle(name, "#00AAAAAA", stripe);
            label->setBgcolor(QColor(r, g, b));
            gl->addWidget(label, row, col);
        }
    }

    setStyleSheet(R"(QLabel { qproperty-alignment: AlignCenter })");
}

MyWidget::~MyWidget() {
    delete ui;
}

void MyWidget::closeEvent(QCloseEvent *event) {
    qDebug() << event->isAccepted();
}

void MyWidget::timerEvent(QTimerEvent *) {
    qDebug() << QThread::currentThread();
}

void MyWidget::initializeUi() {
    ui->setupUi(this);
}

void MyWidget::handleEvents() {

}

