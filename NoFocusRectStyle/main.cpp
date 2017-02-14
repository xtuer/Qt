#include "Widget.h"
#include "NoFocusRectStyle.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    NoFocusRectStyle *style = new NoFocusRectStyle(app.style());
    app.setStyle(style); // Ownership of the style object is transferred to QApplication

    Widget w;
    w.show();

    return app.exec();
}
