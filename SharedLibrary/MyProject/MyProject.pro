include(../common.pri)

QT      = gui widgets
CONFIG -= app_bundle

# 库文件引入: 在输出目录
LIBS += -L$$bin -lMyLibrary

SOURCES += main.cpp
