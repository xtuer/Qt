#include "ui/MainWidget.h"
#include "util/Global.h"
#include "ui/LoginDialog.h"
#include "ui/SetupDialog.h"
#include "util/Settings.h"
#include "ui/RegisterDialog.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 使系统对话框中文化
    QTranslator translator;
    translator.load("qt_zh_CN.qm");
    app.installTranslator(&translator);

    // 注册窗口:
    {
        RegisterDialog dialog;
        dialog.checkRegister();
    }

    {
        LoginDialog loginDialog;
        loginDialog.setWindowFlags(Qt::WindowCloseButtonHint);
        if (QDialog::Accepted != loginDialog.exec()) {
            return 1;
        }
    }

    // Backup the files.
    Global::backup();

    MainWidget w;
    w.show();

    int result = app.exec();
    Global::backup();
    Settings::release();

    return result;
}
