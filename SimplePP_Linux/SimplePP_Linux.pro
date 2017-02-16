TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG= debug_and_release
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS_DEBUG += -gdwarf-3


SOURCES += main.cpp \
    Sim_900.cpp \
    init.cpp \
    Source.cpp \
    handlers.cpp \
    rs232.c \
    variables.cpp

OTHER_FILES += \
    ReadMe.txt \
    doc.txt \
    bitscode.txt

HEADERS += \
    Sim_900.h \
    config.h \
    GSM.h \
    structs.h \
    commands_AT.h \
    commands.h \
    rs232.h \
    includes.h

