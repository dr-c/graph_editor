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
    edge.cpp \
    graph_view.cpp \
    graph_scene.cpp

HEADERS  += mainwindow.h \
    graph.h \
    node.h \
    edge.h \
    graph_view.h \
    graph_scene.h

FORMS    += mainwindow.ui
