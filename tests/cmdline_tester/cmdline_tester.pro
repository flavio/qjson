TEMPLATE = app
TARGET =
CONFIG += link_prl
DEPENDPATH += .
win32 {
  LIBS += -L../../lib -lqjson0
} else {
  LIBS += -L../../lib -lqjson
}
INCLUDEPATH +=  . \
                ../../src

# Input
SOURCES += cmdline_tester.cpp cmdlineparser.cpp
