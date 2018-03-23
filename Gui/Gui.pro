#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T17:44:13
#
#-------------------------------------------------

QT       += core gui network xmlpatterns charts sql multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Gui
TEMPLATE = app
ICON     = AppIcon.icns
CONFIG  -= app_bundle

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

include (/usr/local/qwt-6.1.3/features/qwt.prf)

SOURCES += \
    main.cpp \
    Widget.cpp \
    Pandora.cpp \
    ImageWidget.cpp \
    ImageEffects.cpp \
    Form.cpp \
    MyLayoutItem.cpp \
    MyWidget.cpp \
    SelectionWidget.cpp \
    Chart.cpp \
    ChartView.cpp

FORMS += \
    ImageWidget.ui \
    Form.ui \
    MyWidget.ui

HEADERS += \
    Widget.h \
    Pandora.h \
    ImageWidget.h \
    ImageEffects.h \
    Form.h \
    MyLayoutItem.h \
    MyWidget.h \
    Job.h \
    SelectionWidget.h \
    Chart.h \
    ChartView.h

