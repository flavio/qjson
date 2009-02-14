SET(
	TESTS
	testjsondriver
	cmdline_tester
)

##### Probably don't want to edit below this line ##### 

# Find Qt4
FIND_PACKAGE( Qt4 REQUIRED )

# Pick modules
SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

# Use it
INCLUDE( ${QT_USE_FILE} )

# Include the library include directories, and the current build directory (moc)
INCLUDE_DIRECTORIES(
	../lib
	${CMAKE_CURRENT_BINARY_DIR}
)

# Build the tests
INCLUDE(AddFileDependencies)
FOREACH(test ${TESTS})
	MESSAGE(STATUS "Building ${test}")
	QT4_WRAP_CPP(MOC_SOURCE ${test}.cpp)
	ADD_EXECUTABLE(
		${test}
		${test}.cpp

	)
	ADD_FILE_DEPENDENCIES(${test}.cpp ${MOC_SOURCE})
	TARGET_LINK_LIBRARIES(
		${test}
		${QT_LIBRARIES}
		qjson
	)
ENDFOREACH()
