#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T15:17:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Steps
TEMPLATE = app

CONFIG     -= app_bundle

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

SOURCES += \
        main.cpp \
    UiUtil.cpp \
    StepWidget.cpp

HEADERS += \
    UiUtil.h \
    StepWidget.h

RESOURCES += \
    resources.qrc
