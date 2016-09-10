#-------------------------------------------------
#
# Project created by QtCreator 2016-09-08T18:32:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FramelessWindow
TEMPLATE = app

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

ICON        = AppIcon.icns

SOURCES += main.cpp \
    FramelessWindow.cpp \
    FramelessWindowCentralWidget.cpp

HEADERS  += \
    FramelessWindow.h \
    FramelessWindowCentralWidget.h

FORMS    += \
    FramelessWindowCentralWidget.ui
