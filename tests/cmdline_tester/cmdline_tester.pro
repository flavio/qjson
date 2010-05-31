TEMPLATE = app
TARGET = 
DEPENDPATH += .
LIBS += -L../../lib -lqjson
INCLUDEPATH +=  . \
                ../../src

# Input
SOURCES += cmdline_tester.cpp
