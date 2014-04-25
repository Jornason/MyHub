TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Attribute.cpp \
    DecisionTree.cpp \
    Example.cpp \
    UserInterface.cpp \
    Trainer.cpp \
    Classifier.cpp \
    Printer.cpp \
    Report.cpp \
    Node.cpp

HEADERS += \
    Attribute.h \
    DecisionTree.h \
    Example.h \
    Trainer.h \
    Classifier.h \
    Printer.h \
    Report.h \
    Node.h

OTHER_FILES += \
    car.meta \
    car.test \
    car.train \
    nursery.meta \
    nursery.mixed \
    weather.meta \
    weather.test \
    weather.train \
    car.inst \
    car.output

#QMAKE_CXXFLAGS += -std=c++11
