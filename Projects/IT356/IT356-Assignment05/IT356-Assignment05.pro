#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T09:20:56
#
#-------------------------------------------------
QT       += core xml
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
INCLUDEPATH +="C:\Users\Thomas\Dropbox\C++\QTFiles\include"
LIBS += "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\freeglut.lib" "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\glew32.lib" "C:\Users\Thomas\Dropbox\C++\QTFiles\lib\FreeImage.lib"
SOURCES += main.cpp \
    View.cpp \
    Texture.cpp \
    Sphere.cpp \
    Scenegraph.cpp \
    Plane.cpp \
    Material.cpp \
    Light.cpp \
    Cylinder.cpp \
    Cone.cpp \
    Box.cpp \
    Node.cpp \
    Group.cpp \
    Object.cpp \
    ObjectXMLReader.cpp
OTHER_FILES += \
    white.png \
    lighting-texturing.vert \
    lighting-texturing.frag \
    bare.vert \
    bare.frag \
    earthmap.jpg \
    fermatspiral.txt \
    ../../SharedResources/checkerboard.jpg
HEADERS += \
    View.h \
    Texture.h \
    Sphere.h \
    Scenegraph.h \
    Plane.h \
    ObjectXMLReader.h \
    Object.h \
    Node.h \
    Material.h \
    Light.h \
    Group.h \
    FreeImage.h \
    Cylinder.h \
    Cone.h \
    Box.h \
    HitRecord.h \
    Ray.h
