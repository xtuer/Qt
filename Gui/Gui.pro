#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T17:44:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gui
TEMPLATE = app

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output/obj
MOC_DIR     = $$output/moc
RCC_DIR     = $$output/rcc
UI_DIR      = $$output/ui

SOURCES += main.cpp\
        widget.cpp \
    OxyWidget.cpp

HEADERS  += widget.h \
    OxyWidget.h

FORMS    += widget.ui \
    OxyWidget.ui
