#include <QApplication>
#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QStringList>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // [1] 加载样式
    {
        QFile qssFile("/Users/Biao/Desktop/style.qss");
        qssFile.open(QIODevice::ReadOnly);
        a.setStyleSheet(qssFile.readAll());
    }

    // [2] 创建按钮
    QStringList names = QStringList() << "Java" << "C++" << "Python" << "PHP" << "Qt";
    QButtonGroup *buttonGroup = new QButtonGroup();

    for (int i = 0; i < names.length(); ++i) {
        QPushButton *button = new QPushButton(names.at(i));
        button->setProperty("class", "GroupButton");
        button->setCheckable(true);
        buttonGroup->addButton(button);
    }

    buttonGroup->setExclusive(true);
    buttonGroup->buttons().at(0)->setChecked(true);
    buttonGroup->buttons().at(0)->setProperty("position", "first");
    buttonGroup->buttons().at(names.length() - 1)->setProperty("position", "last");

    // [3] 布局按钮
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(0);

    for (QAbstractButton *button : buttonGroup->buttons()) {
        layout->addWidget(button);
    }

    // [4] 显示窗口
    QWidget window;
    window.setLayout(layout);
    window.resize(500, 150);
    window.show();

    return a.exec();
}
