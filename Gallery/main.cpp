#include "Gallery.h"
#include <QApplication>
#include <QHBoxLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QList<QString> imagePaths = QList<QString>() << "/Users/Biao/Documents/workspace/Qt/Gallery/1.jpg"
                                                 << "/Users/Biao/Documents/workspace/Qt/Gallery/2.jpg"
                                                 << "/Users/Biao/Documents/workspace/Qt/Gallery/3.jpg"
                                                 << "/Users/Biao/Documents/workspace/Qt/Gallery/4.jpg";
    Gallery *gallery = new Gallery(400, 200, imagePaths);

    QWidget *window = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(gallery);
    window->setLayout(layout);
    window->show();

    return a.exec();
}
