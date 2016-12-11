#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T09:58:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PainterStatus
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

SOURCES += main.cpp \
    MainWidget.cpp \
    CodeStatusWidget.cpp \
    ApiStatusWidget.cpp

HEADERS  += \
    MainWidget.h \
    CodeStatusWidget.h \
    ApiStatusWidget.h

FORMS    += \
    MainWidget.ui \
    CodeStatusWidget.ui \
    ApiStatusWidget.ui
