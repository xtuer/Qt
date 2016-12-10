#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T18:44:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Path
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
    MainWidget.cpp \
    TextPathWidget.cpp \
    AnimationAlongPathWidget.cpp \
    ComplicatedPathWidget.cpp

HEADERS  += \
    MainWidget.h \
    TextPathWidget.h \
    AnimationAlongPathWidget.h \
    ComplicatedPathWidget.h

FORMS    += \
    MainWidget.ui \
    TextPathWidget.ui \
    ComplicatedPathWidget.ui
