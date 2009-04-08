PROJECT(qjson)

# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})


# compile in debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ENDIF("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")

# Find Qt4
FIND_PACKAGE( Qt4 REQUIRED )

SET( QT_DONT_USE_QTGUI TRUE )

#add extra search paths for libraries and includes
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
SET (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")


set(QJSON_LIB_MAJOR_VERSION "0")
set(QJSON_LIB_MINOR_VERSION "5")
set(QJSON_LIB_PATCH_VERSION "1")

set(QJSON_LIB_VERSION_STRING "${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}" )

# Include the cmake file needed to use qt4
INCLUDE( ${QT_USE_FILE} )

# Subdirs
ADD_SUBDIRECTORY(lib)
IF (KDE4_BUILD_TESTS)
  ADD_SUBDIRECTORY(tests)
ENDIF (KDE4_BUILD_TESTS)



#CONFIGURE_FILE(
#  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)

#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
