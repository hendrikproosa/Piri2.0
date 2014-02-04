
TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets core gui
INCLUDEPATH  += ../../libs/PiriLib/source
HEADERS      += open.h

SOURCES      += open.cpp \

TARGET        = open
DESTDIR       = ../../bin/plugins

target.path = ../../bin/plugins
INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libs/PiriLib/libs/ -lPiriLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libs/PiriLib/libs/ -lPiriLibd
else:unix: LIBS += -L$$PWD/../../libs/PiriLib/libs/ -lPiriLib

INCLUDEPATH += $$PWD/../../libs/PiriLib/libs
DEPENDPATH += $$PWD/../../libs/PiriLib/libs
