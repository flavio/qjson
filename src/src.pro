# qca qmake profile

QJSON_BASE = ..
QJSON_SRCBASE = .

TEMPLATE = lib
QT      -= gui
TARGET   = qjson
DESTDIR  = $$QJSON_BASE/lib
windows:DLLDESTDIR = $$QJSON_BASE/bin
CONFIG += create_prl

VERSION = 0.7.1

# unix:include($$QJSON_BASE/conf.pri)
# windows:include($$QJSON_BASE/conf_win.pri)

# CONFIG += create_prl
# windows:!staticlib:DEFINES += QJSON_MAKEDLL
# staticlib:PRL_EXPORT_DEFINES += QJSON_STATIC

QJSON_CPP = $$QJSON_SRCBASE
INCLUDEPATH += $$QJSON_CPP

PRIVATE_HEADERS += \
  json_parser.hh \
  json_scanner.h \
  location.hh \
  parser_p.h  \
  position.hh \
  qjson_debug.h  \
  stack.hh

PUBLIC_HEADERS += \
  parser.h \
  parserrunnable.h \
  qobjecthelper.h \
  serializer.h \
  serializerrunnable.h \
  qjson_export.h

HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS

# do support first
SOURCES += \
  json_parser.cc \
  json_scanner.cpp \
  parser.cpp \
  parserrunnable.cpp \
  qobjecthelper.cpp \
  serializer.cpp \
  serializerrunnable.cpp

!debug_and_release|build_pass {
  CONFIG(debug, debug|release) {
    windows:TARGET = $$member(TARGET, 0)d
  }
}

