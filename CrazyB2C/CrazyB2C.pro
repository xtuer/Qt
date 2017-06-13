#-------------------------------------------------
#
# Project created by QtCreator 2016-09-18T14:04:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = CrazyB2C
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
    HttpClient.cpp \
    MainWidget.cpp

HEADERS  += \
    HttpClient.h \
    MainWidget.h

FORMS    += \
    MainWidget.ui

DISTFILES += \
    urls.txt
