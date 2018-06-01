#ifndef SCATTERMAP_H
#define SCATTERMAP_H

#include <QWidget>
#include <QList>

class Scatter;

class ScatterMap : public QWidget {
    Q_OBJECT
public:
    explicit ScatterMap(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void removeScatter(Scatter *scatter);

private:
    QList<Scatter *> scatters;
};

#endif // SCATTERMAP_H
