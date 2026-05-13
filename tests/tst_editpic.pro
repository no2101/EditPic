QT += testlib gui

TARGET = tst_editpic
CONFIG += c++17 qt console warn_on testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_processorthread.cpp \
    $$PWD/../processorthread.cpp

HEADERS += \
    $$PWD/../processorthread.h \
    $$PWD/../typedef.h

INCLUDEPATH += $$PWD/..
