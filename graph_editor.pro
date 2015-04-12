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
    graph_view.cpp \
    graph_scene.cpp \
    graph_scene_mode.cpp \
    qgraphics_node.cpp \
    qgraphics_edge.cpp \
    qgraphics_ellipse_node.cpp \
    qgraphics_simple_line_edge.cpp \
    qgraphics_rounded_rect_node.cpp \
    weight_text_item.cpp \
    qgraphics_cubic_arrow_edge.cpp

HEADERS  += mainwindow.h \
    graph.h \
    node.h \
    edge.h \
    graph_view.h \
    graph_scene.h \
    graph_scene_mode.h \
    qgraphics_node.h \
    item_info.h \
    qgraphics_edge.h \
    qgraphics_ellipse_node.h \
    qgraphics_simple_line_edge.h \
    qgraphics_rounded_rect_node.h \
    weight_text_item.h \
    qgraphics_cubic_arrow_edge.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
