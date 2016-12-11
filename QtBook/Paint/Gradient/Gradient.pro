#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T21:19:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gradient
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
        LinearGradientWidget.cpp \
    LinearGradientAlgorithmWidget.cpp \
    MainWidget.cpp \
    RadialGradientWidget.cpp \
    ConicalGradientWidget.cpp \
    LinearGradientAlgorithm2Widget.cpp

HEADERS  += LinearGradientWidget.h \
    LinearGradientAlgorithmWidget.h \
    MainWidget.h \
    RadialGradientWidget.h \
    ConicalGradientWidget.h \
    LinearGradientAlgorithm2Widget.h

FORMS    += LinearGradientWidget.ui \
    LinearGradientAlgorithmWidget.ui \
    MainWidget.ui \
    RadialGradientWidget.ui \
    ConicalGradientWidget.ui \
    LinearGradientAlgorithm2Widget.ui
