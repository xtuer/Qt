#include "ReportWizardWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReportWizardWidget w;
    w.show();

    return a.exec();
}
