#-------------------------------------------------
#
# Project created by QtCreator 2014-02-01T13:56:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Piri
TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH = $$PWD/../libs/PiriLib/source

win32: LIBS += -L$$PWD/../libs/PiriLib/libs/ -lPiriLib

INCLUDEPATH += $$PWD/../libs/PiriLib/libs
DEPENDPATH += $$PWD/../libs/PiriLib/libs
