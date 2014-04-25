#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T14:27:37
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
    Sphere.cpp \
    Plane.cpp \
    Cylinder.cpp \
    Box.cpp \
    Cone.cpp \
    animator.cpp \
    node.cpp \
    group.cpp \
    Object.cpp \
    ObjectXMLReader.cpp \
    scenegraph.cpp \
    Material.cpp \
    Light.cpp
HEADERS += \
    View.h \
    Sphere.h \
    Plane.h \
    ObjectXMLReader.h \
    Object.h \
    Cylinder.h \
    Box.h \
    Cone.h \
    node.h \
    group.h \
    scenegraph.h \
    animator.h \
    Material.h \
    Light.h

OTHER_FILES += \
    face-with-box.txt \
    face-no-cap.txt \
    face.txt \
    face-hierarchy.txt \
    simple.txt \
    locomotiveold.txt \
    face-hierarchy-lights-textures.txt \
    face-hierarchy-lights.txt \
    locomotiveold2.txt \
    locomotive.txt \
    phong-multiple.vert \
    phong-multiple.frag \
    locomotive-no-texture-or-lights.txt \
    locomotive-simple.txt \
    ../../SharedResources/locomotive-lights-textures.txt

