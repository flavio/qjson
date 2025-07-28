SET (qjson_test_support_SRCS person.cpp person.h)

ADD_LIBRARY (qjson_test_support STATIC ${qjson_test_support_SRCS}
                                       ${qjson_test_support_MOC_SRCS})

target_link_libraries(qjson_test_support Qt::Core)

SET( UNIT_TESTS
  testqobjecthelper
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
    qjson_test_support
  )

  target_include_directories(${test} PRIVATE ../../include)

  if (QJSON_TEST_OUTPUT STREQUAL "xml")
    # produce XML output
    add_unittest(${test} ${test} -xml -o ${test}.tml)
  else (QJSON_TEST_OUTPUT STREQUAL "xml")
    add_unittest(${test} ${test})
  endif (QJSON_TEST_OUTPUT STREQUAL "xml")
ENDFOREACH()
