find_package (CppUnit REQUIRED)
include_directories (
	${CPPUNIT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_executable (chk
	chk.cpp
	singleton.cpp
)
target_link_libraries (chk
	${CPPUNIT_LIBRARY}
)
