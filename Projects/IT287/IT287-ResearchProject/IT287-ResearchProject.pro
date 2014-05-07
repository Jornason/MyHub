#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T14:27:37
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = IT287-ResearchProject
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH +="../../../include"
LIBS += "../lib/freeglut.lib" "../lib/glew32.lib" "../lib/FreeImage.lib"
SOURCES += \
    View.cpp \
    mesh.cpp \
    main.cpp \
    object.cpp

HEADERS += \
    View.h \
    mesh.h \
    object.h

OTHER_FILES += \
    ../../../SharedResources/triangles.frag \
    ../../../SharedResources/trianglesV2.vert \
    ../../../SharedResources/gourd.obj.txt \
    ../../../SharedResources/cow.obj.txt \
    ../../../SharedResources/pumpkin.obj.txt \
    ../../../SharedResources/a10.obj \
    ../../../SharedResources/alfa147.obj \
    ../../../SharedResources/apple.obj \
    ../../../SharedResources/balljoint-small.obj \
    ../../../SharedResources/cow.obj \
    ../../../SharedResources/cow-nonormals.obj \
    ../../../SharedResources/custom.obj \
    ../../../SharedResources/gourd.obj \
    ../../../SharedResources/hip-small.obj \
    ../../../SharedResources/icosahedron.obj \
    ../../../SharedResources/octahedron.obj \
    ../../../SharedResources/pumpkin.obj \
    ../../../SharedResources/teapot.obj \
    ../../../SharedResources/teddybear.obj \
    ../../../SharedResources/slices.txt
