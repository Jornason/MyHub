TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Attribute.cpp \
    Classifier.cpp \
    Example.cpp \
    Printer.cpp \
    Report.cpp \
    Trainer.cpp \
    UserInterface.cpp \
    ProbabilityTable.cpp

HEADERS += \
    Attribute.h \
    Classifier.h \
    Example.h \
    Printer.h \
    Report.h \
    Trainer.h \
    ProbabilityTable.h

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
