ADD_EXECUTABLE(
  cmdline_tester
  cmdline_tester.cpp
  cmdlineparser.cpp
)

target_include_directories(cmdline_tester PRIVATE ../../include)

TARGET_LINK_LIBRARIES(
  cmdline_tester
  Qt::Core
  Qt::Widgets
  Qt::Test
  qjson${QJSON_SUFFIX}
)
