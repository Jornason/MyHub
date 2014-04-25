#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T09:53:04
#
#-------------------------------------------------
QT       += core
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    View.cpp
INCLUDEPATH +="C:\Users\Thomas\Dropbox\C++\QTFiles\include"
LIBS += "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\freeglut.lib" "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\glew32.lib" "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\FreeImage.lib"
OTHER_FILES += \
    ../../SharedResources/triangles.vert \
    ../../SharedResources/triangles.frag \
    ../../SharedResources/trianglesV1.vert
HEADERS += \
    View.h
