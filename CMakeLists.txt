CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
PROJECT(qjson)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

add_compile_definitions("QT_NO_CAST_FROM_ASCII" "DQT_NO_CAST_TO_ASCII")

IF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  add_compile_definitions("QT_NO_DEBUG_OUTPUT")
ENDIF()

# Ability to disable verbose debug output
IF(QJSON_VERBOSE_DEBUG_OUTPUT)
  add_compile_definitions("QJSON_VERBOSE_DEBUG_OUTPUT")
endif()

# On Windows debug library should have 'd' postfix.
IF (WIN32)
  SET(CMAKE_DEBUG_POSTFIX "d")
elseif (APPLE)
  set(CMAKE_DEBUG_POSTFIX "_debug")
endif ()

option(QJSON_BUILD_TESTS "Build tests" OFF)

# BUILD_SHARED_LIBS is cmake variable. Need to change default value.
option(BUILD_SHARED_LIBS "Build shared library" ON)

OPTION(OSX_FRAMEWORK "Build a Mac OS X Framework")

# Allow to use shared Qt when compiling static version of QJSON.
# Has effect only when BUILD_SHARED_LIBS is OFF.
option(LINK_SHARED_QT "Force to link with shared Qt" OFF)

SET(FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Library/Frameworks"
    CACHE PATH "Where to place qjson.framework if OSX_FRAMEWORK is selected")

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core)

set(QJSON_SUFFIX "-qt6")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_compile_definitions("QT_USE_QSTRINGBUILDER")
MESSAGE("Enable QStringBuilder")

# IF (NOT WIN32)
#   SET( QT_DONT_USE_QTGUI TRUE )
# ENDIF()

#add extra search paths for libraries and includes
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
SET (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")
SET (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
SET (CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${CMAKE_PROJECT_NAME}${QJSON_SUFFIX}" CACHE PATH "Directory where to install QJSONConfig.cmake")

set(QJSON_LIB_MAJOR_VERSION "0")
set(QJSON_LIB_MINOR_VERSION "10")
set(QJSON_LIB_PATCH_VERSION "0")

set(QJSON_LIB_VERSION_STRING "${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )

# pkg-config
IF (NOT WIN32)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/QJson.pc.in
                  ${CMAKE_CURRENT_BINARY_DIR}/QJson${QJSON_SUFFIX}.pc
                  @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/QJson${QJSON_SUFFIX}.pc
           DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF (NOT WIN32)

# Subdirs
ADD_SUBDIRECTORY(src)
IF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)
  enable_testing()
  ADD_SUBDIRECTORY(tests)
ENDIF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

INSTALL(EXPORT qjson-export DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE QJSON${QJSON_SUFFIX}Targets.cmake)

# figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into the QJSONConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX} )

# cmake-modules
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/QJSONConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/QJSON${QJSON_SUFFIX}Config.cmake
               @ONLY)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/QJSONConfigVersion.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/QJSON${QJSON_SUFFIX}ConfigVersion.cmake
               @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/QJSON${QJSON_SUFFIX}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/QJSON${QJSON_SUFFIX}ConfigVersion.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}")

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
