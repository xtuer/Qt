#include <QApplication>
#include <QDebug>
#include "ImageWidget.h"
#include <QStringList>
#include <QUrl>
#include "Form.h"

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[]) {
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

//    ImageWidget w;
//    w.show();

    Form form;
    form.show();

    QObject::connect(&form, &Form::fox, []{
        qDebug() << "fox";
    });

    return app.exec();
}
