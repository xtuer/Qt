#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QtCore/qglobal.h>

#if defined(BUILD_LIB)
#   define LIB Q_DECL_EXPORT
#else
#   define LIB Q_DECL_IMPORT
#endif

namespace Ui {
class Form;
}

class LIB Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;
};

#endif // FORM_H
