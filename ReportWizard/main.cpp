#include <QApplication>
#include "gui-report/ReportWizard.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ReportWizard w;
    w.show();

    return a.exec();
}
