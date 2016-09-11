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

ICON        = AppIcon.icns

SOURCES += main.cpp\
        widget.cpp \
    OxyWidget.cpp \
    FramelessWidget.cpp \
    Person.cpp

HEADERS  += widget.h \
    OxyWidget.h \
    FramelessWidget.h \
    Person.h

FORMS    += widget.ui \
    OxyWidget.ui
