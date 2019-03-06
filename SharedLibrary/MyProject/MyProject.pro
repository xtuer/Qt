include(../common.pri)

QT     -= gui
CONFIG += c++11 console
CONFIG -= app_bundle

# 头文件目录: 在源码目录
INCLUDEPATH += $$PWD/../MyLibrary

# 库文件引入: 在输出目录
LIBS += -L$$bin -lMyLibrary

SOURCES += main.cpp
