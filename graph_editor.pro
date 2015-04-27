#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T14:23:55
#
#-------------------------------------------------

QT       += core gui xml

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
    qgraphics_cubic_arrow_edge.cpp \
    history.cpp \
    graph_creation_dialog.cpp \
    pen_chooser_frame.cpp \
    pen_chooser_dialog.cpp \
    pen_observer_frame.cpp \
    brush_chooser_frame.cpp \
    brush_chooser_dialog.cpp \
    brush_observer_frame.cpp \
    file_graph_manager.cpp

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
    qgraphics_cubic_arrow_edge.h \
    history.h \
    graph_creation_dialog.h \
    pen_chooser_frame.h \
    pen_chooser_dialog.h \
    pen_observer_frame.h \
    brush_chooser_frame.h \
    brush_chooser_dialog.h \
    brush_observer_frame.h \
    graph_configuration.h \
    file_graph_manager.h

FORMS    += mainwindow.ui \
    graph_creation_dialog.ui \
    pen_chooser_frame.ui \
    pen_chooser_dialog.ui \
    brush_chooser_frame.ui \
    brush_chooser_dialog.ui

RESOURCES += \
    icons.qrc
