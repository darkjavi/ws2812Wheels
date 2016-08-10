#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T14:00:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LWControl
TEMPLATE = app


SOURCES += main.cpp\
        controlmainwindow.cpp \
    connectwidget.cpp \
    sensorswidget.cpp \
    gwidgettxrx.cpp \
    socketclient.cpp

HEADERS  += controlmainwindow.h \
    connectwidget.h \
    sensorswidget.h \
    gwidgettxrx.h \
    socketclient.h

FORMS    += controlmainwindow.ui \
    connectwidget.ui \
    sensorswidget.ui

RESOURCES += \
    resources.qrc
