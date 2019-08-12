#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T15:52:06
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Ano
TEMPLATE = app

CONFIG  += c++11
RC_ICONS = AppIcon.ico  # For Win
ICON     = AppIcon.icns # For Mac

DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = $$TARGET'_d'
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += \
        Callout.cpp \
        CalloutChartWidget.cpp \
        main.cpp

HEADERS += \
        Callout.h \
        CalloutChartWidget.h

FORMS += \
    CalloutChartWidget.ui
