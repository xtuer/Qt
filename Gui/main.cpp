#include "FramelessWidget.h"
#include "widget.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建包含主要控件的 Widget
//    Widget w;
//    w.show();

    QString str("Niu 上帝");
    QByteArray buf = str.toLatin1();
    const char *chs = buf.data();
    qDebug() << chs;

    return app.exec();
}
