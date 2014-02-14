#-------------------------------------------------
#
# Project created by QtCreator 2014-02-01T14:10:59
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiriLib
TEMPLATE = lib

DEFINES += PIRILIB_LIBRARY

SOURCES += pirilib.cpp \
    mainwindow.cpp \
    nodegraph.cpp \
    viewernodegraph.cpp \
    node.cpp \
    knobcallback.cpp \
    op.cpp \
    edge.cpp \
    miconnect.cpp \
    knobs.cpp \
    searchdialog.cpp


HEADERS += pirilib.h\
        pirilib_global.h \
    mainwindow.h \
    nodegraph.h \
    viewernodegraph.h \
    interfaces.h \
    node.h \
    knobcallback.h \
    op.h \
    edge.h \
    miconnect.h \
    knobs.h \
    searchdialog.h

