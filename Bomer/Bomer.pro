QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bomer
CONFIG -= app_bundle

TEMPLATE = app

# Output directory
CONFIG(debug, debug|release) {
    compiled = debug
}
CONFIG(release, debug|release) {
    compiled = release
}

DESTDIR     = bin
OBJECTS_DIR = $$compiled
MOC_DIR     = $$compiled
RCC_DIR     = $$compiled
UI_DIR      = $$compiled

SOURCES += main.cpp \
    BomUtils.cpp \
    FileUtils.cpp \
    MainWidget.cpp

HEADERS += \
    BomUtils.h \
    FileUtils.h \
    MainWidget.h

FORMS += \
    MainWidget.ui
