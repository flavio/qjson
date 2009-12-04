PROJECT(qjson)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII" )

# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# compile in debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")

# Ability to disable verbose debug output
IF(QJSON_VERBOSE_DEBUG_OUTPUT)
  ADD_DEFINITIONS("-DQJSON_VERBOSE_DEBUG_OUTPUT")
endif(QJSON_VERBOSE_DEBUG_OUTPUT)

# Find Qt4
FIND_PACKAGE( Qt4 REQUIRED )

SET( QT_DONT_USE_QTGUI TRUE )

#add extra search paths for libraries and includes
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
SET (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")
SET (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
SET (CMAKE_MODULES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/apps/cmake/modules" CACHE PATH "The directory to install FindQRpc.cmake to")


set(QJSON_LIB_MAJOR_VERSION "0")
set(QJSON_LIB_MINOR_VERSION "7")
set(QJSON_LIB_PATCH_VERSION "1")

set(QJSON_LIB_VERSION_STRING "${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}" )

# pkg-config
IF (NOT WIN32)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/QJson.pc.in
                  ${CMAKE_CURRENT_BINARY_DIR}/QJson.pc
                  @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/QJson.pc
           DESTINATION lib${LIB_SUFFIX}/pkgconfig)
ENDIF (NOT WIN32)

# Include the cmake file needed to use qt4
INCLUDE( ${QT_USE_FILE} )

# Subdirs
ADD_SUBDIRECTORY(src)
IF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)
  enable_testing()
  ADD_SUBDIRECTORY(tests)
ENDIF (KDE4_BUILD_TESTS OR QJSON_BUILD_TESTS)

add_subdirectory(cmake/modules)


#CONFIGURE_FILE(
#  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)

#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
