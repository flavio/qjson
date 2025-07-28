SET( UNIT_TESTS
  testscanner
)

# Build the tests
FOREACH(test ${UNIT_TESTS})
  MESSAGE(STATUS "Building ${test}")
  ADD_EXECUTABLE(
    ${test}
    ${test}.cpp
  )

  if(WIN32 AND BUILD_SHARED_LIBS)
    set(QJSON_SCANNER qjson_scanner)
  endif()

  TARGET_LINK_LIBRARIES(
    ${test}
    Qt::Core
    Qt::Test
    qjson${QJSON_SUFFIX}
    ${QJSON_SCANNER}
  )

  target_include_directories(${test} PRIVATE ../../src ../../include)

  if (QJSON_TEST_OUTPUT STREQUAL "xml")
    # produce XML output
    add_unittest(${test} ${test} -xml -o ${test}.tml)
  else (QJSON_TEST_OUTPUT STREQUAL "xml")
    add_unittest(${test} ${test})
  endif (QJSON_TEST_OUTPUT STREQUAL "xml")
ENDFOREACH()
