include(../common.pri)

QT      = gui widgets
CONFIG -= app_bundle

# 库文件引入: 在输出目录
LIBS += -L$$bin -lLib-1
LIBS += -L$$bin -lLib-2

SOURCES += main.cpp
