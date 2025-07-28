find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets Test)

macro(add_unittest DESCRIPTION)
    add_test(NAME "${DESCRIPTION}"
             WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
             COMMAND ${ARGN}
    )
endmacro()

ADD_SUBDIRECTORY(cmdline_tester)
ADD_SUBDIRECTORY(parser)
ADD_SUBDIRECTORY(scanner)
ADD_SUBDIRECTORY(qobjecthelper)
ADD_SUBDIRECTORY(serializer)
