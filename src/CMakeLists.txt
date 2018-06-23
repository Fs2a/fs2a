# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018
# vim:set ts=2 sw=2 noexpandtab:

include_directories (
	${PQXX_INCLUDE_DIRS}
)

add_library (fs2a SHARED
	ConnPool.cpp
	Logger.cpp
	Tracer.cpp
)

target_link_libraries (fs2a
	${CMAKE_THREAD_LIBS_INIT}
	${PQXX_LIBRARIES}
)
