TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
TARGET = ResearchProject

SOURCES += \
    FastTriangulator.cpp \
    GraphicsGemsTriangulator.cpp \
    MeshTest.cpp

HEADERS += \
    FastTriangulator.h \
    GraphicsGemsTriangulator.h \
    Point2D.h \
    Point3D.h \
    Triangulator.h

