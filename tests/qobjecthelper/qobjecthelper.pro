CONFIG  += qtestlib
CONFIG += link_prl
DEFINES += QMAKE_BUILD

HEADERS = person.h

SOURCES = testqobjecthelper.cpp \
          person.cpp

win32 {
  LIBS += -L../../lib -lqjson0
} else {
  LIBS += -L../../lib -lqjson
}
INCLUDEPATH +=  . \
                ../../src
