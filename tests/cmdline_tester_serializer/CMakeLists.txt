##### Probably don't want to edit below this line #####

# Use it
INCLUDE( ${QT_USE_FILE} )

INCLUDE(AddFileDependencies)

# Include the library include directories, and the current build directory (moc)
INCLUDE_DIRECTORIES(
  ../../src
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_EXECUTABLE(
  cmdline_tester_serializer
  cmdline_tester_serializer.cpp
)

TARGET_LINK_LIBRARIES(
  cmdline_tester_serializer
  ${QT_LIBRARIES}
  qjson
)
