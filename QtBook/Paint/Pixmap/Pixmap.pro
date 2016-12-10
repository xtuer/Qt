#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T19:25:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pixmap
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

SOURCES += main.cpp\
        PixmapWidget.cpp

HEADERS  += PixmapWidget.h

FORMS    += PixmapWidget.ui

RESOURCES += \
    resources.qrc
