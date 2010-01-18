SOURCES = testserializer.cpp
CONFIG  += qtestlib

DEFINES += QMAKE_BUILD

LIBS += -L../../lib -lqjson
INCLUDEPATH +=  . \
                ../../src
