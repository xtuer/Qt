#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "../domain/UserData.h"
#include "../util/Global.h"
#include "../util/Settings.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

EditDialog::EditDialog(const QString &title, QWidget *parent) : QDialog(parent), ui(new Ui::EditDialog) {
    ui->setupUi(this);
    setWindowTitle(title);
    ui->kaiShiRiDateEdit->setDate(QDate::currentDate());
    ui->zhiFuDaoDateEdit->setDate(QDate::currentDate().addMonths(1));

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->editModeCheckBox, SIGNAL(toggled(bool)), this, SLOT(changeEditMode(bool)));

    ui->editModeCheckBox->setChecked(false);

    Settings::getInstance().restoreWindowBounds(this, objectName());
}

EditDialog::~EditDialog() {
    Settings::getInstance().saveWindowBounds(*this, objectName());

    delete ui;
}

void EditDialog::setUserData(const UserData &userData) {
    ui->leiXinComboBox->setCurrentText(userData.leiXin);
    ui->shangQuanLineEdit->setText(userData.shangQuan);
    ui->guiShuLineEdit->setText(userData.guiShu);
    ui->zhangHaoLineEdit->setText(userData.zhangHao);
    ui->jiaoYiMiMaLineEdit->setText(userData.jiaoYiMiMa);
    ui->benJingDoubleSpinBox->setValue(Global::stringToDouble(userData.benJing, tr("万")));
    ui->huMingLineEdit->setText(userData.huMing);
    ui->kaiShiRiDateEdit->setDate(QDate::fromString(userData.kaiShiRi, Global::DATE_FORMAT));
    ui->zhiFuDaoDateEdit->setDate(QDate::fromString(userData.zhiFuDao, Global::DATE_FORMAT));
    ui->liXiDoubleSpinBox->setValue(Global::stringToDouble(userData.liXi, tr("%")));
    ui->shiFuLiXiDoubleSpinBox->setValue(userData.ShiFuLiXi.toDouble());
    ui->zhiJingMiMaLineEdit->setText(userData.zhiJingMiMa);
    ui->biLiLineEdit->setText(userData.biLi);
    ui->yongJingLineEdit->setText(userData.yongJing);
    ui->zhuangTaiComboBox->setCurrentText(userData.zhuangTai);
    ui->beiZhuTextEdit->setPlainText(userData.beiZhu);

    ui->reservedLineEdit1->setText(userData.reserved1);
    ui->reservedLineEdit2->setText(userData.reserved2);
    ui->reservedLineEdit3->setText(userData.reserved3);
    ui->reservedLineEdit4->setText(userData.reserved4);
    ui->reservedLineEdit5->setText(userData.reserved5);
    ui->reservedLineEdit6->setText(userData.reserved6);
}

UserData EditDialog::getUserData() const {
    UserData userData;

    double benJin = ui->benJingDoubleSpinBox->value();
    double liXi = ui->liXiDoubleSpinBox->value() / 100.0;

    userData.leiXin     = ui->leiXinComboBox->currentText();
    userData.shangQuan  = ui->shangQuanLineEdit->text();
    userData.guiShu     = ui->guiShuLineEdit->text();
    userData.zhangHao   = ui->zhangHaoLineEdit->text();
    userData.jiaoYiMiMa = ui->jiaoYiMiMaLineEdit->text();
    userData.benJing    = QString::number(ui->benJingDoubleSpinBox->value()) + QString("万");
    userData.huMing     = ui->huMingLineEdit->text();
    userData.kaiShiRi   = ui->kaiShiRiDateEdit->date().toString(Global::DATE_FORMAT);
    userData.zhiFuDao   = ui->zhiFuDaoDateEdit->date().toString(Global::DATE_FORMAT);
    userData.liXi       = QString::number(ui->liXiDoubleSpinBox->value()) + QString("%");
    userData.yinFuLiXi  = QString::number(benJin * liXi) + QString("万");
    userData.ShiFuLiXi  = QString::number(ui->shiFuLiXiDoubleSpinBox->value());
    userData.zhiJingMiMa= ui->zhiJingMiMaLineEdit->text();
    userData.biLi       = ui->biLiLineEdit->text();
    userData.yongJing   = ui->yongJingLineEdit->text();
    userData.zhuangTai  = ui->zhuangTaiComboBox->currentText();
    userData.beiZhu     = ui->beiZhuTextEdit->toPlainText();

    userData.reserved1 = ui->reservedLineEdit1->text();
    userData.reserved2 = ui->reservedLineEdit2->text();
    userData.reserved3 = ui->reservedLineEdit3->text();
    userData.reserved4 = ui->reservedLineEdit4->text();
    userData.reserved5 = ui->reservedLineEdit5->text();
    userData.reserved6 = ui->reservedLineEdit6->text();

    return userData;
}

void EditDialog::changeEditMode(bool editable) {
    ui->frame->setEnabled(editable);
    ui->saveButton->setEnabled(editable);
}

void EditDialog::setReservedLabels(const QStringList &labels) {
    if (labels.size() != 6) { return; }

    ui->reservedLabel1->setText(labels.at(0));
    ui->reservedLabel2->setText(labels.at(1));
    ui->reservedLabel3->setText(labels.at(2));
    ui->reservedLabel4->setText(labels.at(3));
    ui->reservedLabel5->setText(labels.at(4));
    ui->reservedLabel6->setText(labels.at(5));
}

bool EditDialog::checkInput() {
    QString zt = ui->zhuangTaiComboBox->currentText();
    if (zt != tr("在用") && zt != tr("停用")) {
        QMessageBox::warning(this, tr("错误"), tr("状态必须是『在用』或者『停用』中的一个"));
        return false;
    }

    if (ui->shangQuanLineEdit->text().isEmpty() ||
            ui->guiShuLineEdit->text().isEmpty() ||
            ui->zhangHaoLineEdit->text().isEmpty() ||
            ui->jiaoYiMiMaLineEdit->text().isEmpty() ||
            ui->zhiJingMiMaLineEdit->text().isEmpty() ||
            ui->huMingLineEdit->text().isEmpty() ||
            ui->biLiLineEdit->text().isEmpty() ||
            ui->yongJingLineEdit->text().isEmpty() ||
            ui->beiZhuTextEdit->toPlainText().isEmpty()
            ) {
        QMessageBox::warning(this, tr("错误"), tr("请填完所有必填项才能保存"));
        return false;
    }

    return true;
}

void EditDialog::save() {
    if (checkInput()) {
        int result = QMessageBox::question(this, tr("保存"), tr("进行保存 ?"), QMessageBox::Yes | QMessageBox::No);
        if (QMessageBox::Yes == result) {
            accept();
            return;
        }
    }
}

void EditDialog::closeEvent(QCloseEvent *event) {
    if (!ui->editModeCheckBox->isChecked()) {
        event->accept();
        return;
    }

    int result = QMessageBox::question(this, tr("关闭"), tr("关闭窗口吗 ?"), QMessageBox::Yes | QMessageBox::No);

    if (QMessageBox::No == result) {
        event->ignore();
        return;
    }

    result = QMessageBox::question(this, tr("保存"), tr("有数据未保存, 需要保存吗 ?"), QMessageBox::Yes | QMessageBox::No);

    if (QMessageBox::Yes == result) {
        if (checkInput()) {
            accept();
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void EditDialog::keyPressEvent(QKeyEvent *event) {
    if(event->key() != Qt::Key_Escape) QDialog::keyPressEvent(event);
}
