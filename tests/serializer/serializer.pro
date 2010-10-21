SOURCES = testserializer.cpp
CONFIG  += qtestlib
CONFIG += link_prl
DEFINES += QMAKE_BUILD

win32 {
  LIBS += -L../../lib -lqjson0
} else {
  LIBS += -L../../lib -lqjson
}
INCLUDEPATH +=  . \
                ../../src
