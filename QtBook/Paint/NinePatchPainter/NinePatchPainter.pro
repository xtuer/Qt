#-------------------------------------------------
#
# Project created by QtCreator 2017-09-06T14:27:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NinePatchPainter
TEMPLATE = app
CONFIG  -=app_bundle

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
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        Widget.cpp \
    NinePatchPainter.cpp

HEADERS += \
        Widget.h \
    NinePatchPainter.h

FORMS +=

RESOURCES += \
    resources.qrc
