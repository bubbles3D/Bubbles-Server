#-------------------------------------------------
#
# Project created by QtCreator 2012-01-15T18:50:13
#
#-------------------------------------------------

QT       += core
QT       += network xml

QT       -= gui

TARGET = BubblesServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    player.cpp \
    gameengine.cpp \
    field.cpp \
    gameobject.cpp \
    projectile.cpp \
    sphere.cpp \
    logger.cpp \
    cuboid.cpp \
    obstacle.cpp \
    gamedirector.cpp \
    team.cpp

HEADERS += \
    server.h \
    player.h \
    gameengine.h \
    field.h \
    gameobject.h \
    projectile.h \
    sphere.h \
    logger.h \
    cuboid.h \
    obstacle.h \
    gamedirector.h \
    team.h


symbian: LIBS += -lqjson0
else:win32: LIBS += -L$$PWD/../QJson/lib/ -lqjson0
else:unix: LIBS += -lqjson

INCLUDEPATH += $$PWD/../QJson/include
DEPENDPATH += $$PWD/../QJson/include
