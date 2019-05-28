#include <QApplication>
#include <QDebug>
#include <QComboBox>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QComboBox combo;
    combo.addItem("One", "1");
    combo.addItem("Two", "2");
    combo.setCurrentIndex(-1);

    QObject::connect(&combo, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [](const QString &text) {
        qDebug() << text;
    });

    combo.setCurrentIndex(0);

    combo.show();

    return app.exec();
}
