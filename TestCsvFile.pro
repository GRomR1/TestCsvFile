#-------------------------------------------------
#
# Project created by QtCreator 2015-12-20T00:43:19
#
#-------------------------------------------------

QT       += core gui concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestCsvFile
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    csvworker.cpp \
    database.cpp \
    address.cpp

HEADERS  += widget.h \
    csvworker.h \
    database.h \
    defines.h \
    address.h

FORMS    += widget.ui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
