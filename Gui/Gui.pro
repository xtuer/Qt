#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T17:44:13
#
#-------------------------------------------------

QT       += core gui network xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET   = Gui
TEMPLATE = app
ICON     = AppIcon.icns
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
    Widget.cpp \
    MainWindow.cpp

FORMS += \
    Widget.ui \
    MainWindow.ui

HEADERS += \
    Widget.h \
    MainWindow.h
