SET( UNIT_TESTS
  testparser
)

# Build the tests
FOREACH(test ${UNIT_TESTS})
  MESSAGE(STATUS "Building ${test}")
  ADD_EXECUTABLE(
    ${test}
    ${test}.cpp
  )

  TARGET_LINK_LIBRARIES(
    ${test}
    Qt::Core
    Qt::Test
    qjson${QJSON_SUFFIX}
  )

  target_include_directories(${test} PRIVATE ../../include)

  if (QJSON_TEST_OUTPUT STREQUAL "xml")
    # produce XML output
    add_unittest(${test} ${test} -xml -o ${test}.tml)
  else (QJSON_TEST_OUTPUT STREQUAL "xml")
    add_unittest(${test} ${test})
  endif (QJSON_TEST_OUTPUT STREQUAL "xml")
ENDFOREACH()
