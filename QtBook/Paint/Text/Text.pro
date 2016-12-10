#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T10:37:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Text
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
        DrawSimpleTextWidget.cpp \
    CentralTextWidget.cpp \
    WrapTextWidget.cpp \
    AutoResizeTextWidget.cpp \
    MainWidget.cpp \
    FontMetricsWidget.cpp

HEADERS  += DrawSimpleTextWidget.h \
    CentralTextWidget.h \
    WrapTextWidget.h \
    AutoResizeTextWidget.h \
    MainWidget.h \
    FontMetricsWidget.h

FORMS    += DrawSimpleTextWidget.ui \
    CentralTextWidget.ui \
    WrapTextWidget.ui \
    AutoResizeTextWidget.ui \
    MainWidget.ui \
    FontMetricsWidget.ui
