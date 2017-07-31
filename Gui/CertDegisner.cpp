#include "CertDegisner.h"
#include "ui_CertDegisner.h"
#include "DraggableLabel.h"

#include <QPainter>

CertDegisner::CertDegisner(QWidget *parent) : QWidget(parent), ui(new Ui::CertDegisner),
    settings("data.ini", QSettings::IniFormat),
    certPattern(QPixmap("cert-pattern.jpg")) {
    ui->setupUi(this);

    createLabel("name", "姓名");
    createLabel("age",  "年龄");
    createLabel("time", "毕业时间");
    createLabel("major",  "专业");
    createLabel("gender", "性别");
    createLabel("certNo", "证书编号");
    createLabel("school", "北京师范大学");

    connect(ui->saveButton, &QPushButton::clicked, [this] {
        // 保存 label 的 geometry
        foreach (const QString &key, labels.keys()) {
            settings.setValue(key, labels[key]->geometry());
        }
    });
}

CertDegisner::~CertDegisner() {
    delete ui;
}

void CertDegisner::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(20, 20, certPattern);
}

void CertDegisner::createLabel(const QString &key, const QString &value) {
    // 创建 label
    labels.insert(key, new DraggableLabel(value, this));

    // 恢复 label 的 geometry
    if (settings.value(key).isValid()) {
        labels[key]->setGeometry(settings.value(key).toRect());
    }
}
