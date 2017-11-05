QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../MyLibrary
LIBS += -L$$OUT_PWD/../bin -lMyLibrary

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = ../bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += main.cpp
