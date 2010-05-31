CONFIG  += qtestlib

DEFINES += QMAKE_BUILD

HEADERS = person.h

SOURCES = testqobjecthelper.cpp \
          person.cpp

LIBS += -L../../lib -lqjson
INCLUDEPATH +=  . \
                ../../src
