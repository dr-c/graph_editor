#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T14:23:55
#
#-------------------------------------------------

QT       += core gui

CONFIG  +=  c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graph_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    node.cpp \
    edge.cpp

HEADERS  += mainwindow.h \
    graph.h \
    node.h \
    edge.h

FORMS    += mainwindow.ui
