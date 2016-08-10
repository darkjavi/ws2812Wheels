#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24T22:45:10
#
#-------------------------------------------------

QT       += core gui
CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sensorView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sserial.cpp

HEADERS  += mainwindow.h \
    sserial.h

FORMS    += mainwindow.ui

include(qserialport/serialport-lib.pri)
