#ifndef CERTDEGISNER_H
#define CERTDEGISNER_H

#include <QWidget>
#include <QSettings>
#include <QHash>
#include <QPixmap>

class DraggableLabel;

namespace Ui {
class CertDegisner;
}

class CertDegisner : public QWidget {
    Q_OBJECT

public:
    explicit CertDegisner(QWidget *parent = 0);
    ~CertDegisner();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void createLabel(const QString &key, const QString &value);

    Ui::CertDegisner *ui;
    QHash<QString, DraggableLabel*> labels;
    QSettings settings;
    QPixmap certPattern; // 证书模版
};

#endif // CERTDEGISNER_H
