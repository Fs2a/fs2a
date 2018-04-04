# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018
# vim:set ts=2 sw=2 noexpandtab:

find_package (CppUnit REQUIRED)
include_directories (
	${CPPUNIT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_executable (chk
	chk.cpp
	logger.cpp
	observing.cpp
	singleton.cpp
)
target_link_libraries (chk
	${CPPUNIT_LIBRARY}
	fs2a
)
