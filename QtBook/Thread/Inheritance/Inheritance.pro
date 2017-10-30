#-------------------------------------------------
#
# Project created by QtCreator 2017-10-30T20:42:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Inheritance
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
        ReadingWidget.cpp \
    ReadingThread.cpp

HEADERS += \
        ReadingWidget.h \
    ReadingThread.h

FORMS += \
        ReadingWidget.ui
