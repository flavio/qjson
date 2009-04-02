include_directories (./)

# add_custom_command (OUTPUT json_parser.cc
#                    PRE_BUILD
#                    COMMAND bison -t -o json_parser.cc -d json_parser.yy
#                    DEPENDS json_parser.yy
#                    WORKING_DIRECTORY ${qjson_SOURCE_DIR}/src/
#                    )

set (qjson_SRCS json_driver.cc json_scanner.cpp json_parser.cc)

add_library (qjson SHARED ${qjson_SRCS})
target_link_libraries( qjson ${QT_LIBRARIES})
set_target_properties(qjson PROPERTIES DEFINE_SYMBOL MAKE_QJSON_LIB)

install(FILES
        json_driver.hh
        json_parser.hh
        json_scanner.h
        location.hh
        position.hh
        stack.hh
        qjson_export.h
        DESTINATION include/qjson
)

install(TARGETS qjson DESTINATION ${LIB_DESTINATION})
