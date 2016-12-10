#include "FontMetricsWidget.h"
#include "ui_FontMetricsWidget.h"
#include <QPainter>

FontMetricsWidget::FontMetricsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::FontMetricsWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

FontMetricsWidget::~FontMetricsWidget() {
    delete ui;
}

void FontMetricsWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setFont(QFont("Times", 150, QFont::Bold));
    QFontMetrics metrics = painter.fontMetrics();
    int x = 0;
    int y = metrics.ascent();

    painter.drawText(x, y, "jEh");
}
