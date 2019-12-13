#!/bin/bash

# Find Base repo dir
base=$(readlink -f $(dirname "${BASH_SOURCE[0]}")/..)
prj=$(basename $(readlink -f .))

# Initialize a base development tree

mkdir -p bld chk cmake/modules src
> bld/.keep_as_build_directory

if ! [ -r .gitignore ]; then
cat > .gitignore <<EOF
.*.swp
bld/**
!bld/.keep_as_build_directory
tags
EOF
fi

cp $base/chk/chk.cpp chk
cp $base/src/Logger.* $base/src/Singleton.h src
if ! [ -r ./CMakeLists.txt ]; then
cat >./CMakeLists.txt <<EOF
# vim:set ts=4 sw=4 noexpandtab:

cmake_minimum_required (VERSION 3.0)
if(POLICY CMP0015)
	cmake_policy(SET CMP0015 NEW)
endif()
if(POLICY CMP0056)
	cmake_policy(SET CMP0056 NEW)
endif()

list (APPEND CMAKE_MODULE_PATH "\${CMAKE_SOURCE_DIR}/cmake/modules")

# Set the Git repository root directory
execute_process (
	COMMAND git rev-parse --show-toplevel
	OUTPUT_VARIABLE GITREPOROOT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

project ($prj)

# Custom C/C++ flags
set (CUSTOM_FLAGS   "-Wall -Werror -Wextra -Wno-varargs -DGITREPOROOT='\${GITREPOROOT}'")
set (CUSTOM_DEBUG   "-O0 -g3 -ggdb")
set (CUSTOM_RELEASE "-O3")

# Common linker flags
set (CMAKE_EXE_LINKER_FLAGS_DEBUG   "\${CMAKE_EXE_LINKER_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "\${CMAKE_EXE_LINKER_FLAGS} -s")

# Plain C flags
set (CMAKE_C_FLAGS         "\${CUSTOM_FLAGS}")
set (CMAKE_C_FLAGS_DEBUG   "\${CUSTOM_DEBUG}")
set (CMAKE_C_FLAGS_RELEASE "\${CUSTOM_RELEASE}")

# C++ flags, mostly the same as C
set (CMAKE_CXX_FLAGS         "\${CUSTOM_FLAGS} -std=gnu++1y")
set (CMAKE_CXX_FLAGS_DEBUG   "\${CUSTOM_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE "\${CUSTOM_RELEASE}")

# Find necessary packages
find_package (Boost REQUIRED
	COMPONENTS system
)
find_package (Threads REQUIRED)

# Include stuff for code coverage
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	INCLUDE(CodeCoverage)
	SETUP_TARGET_FOR_COVERAGE(cov$prj chk ${prj}cov)
	add_custom_target(RunCoverage ALL DEPENDS cov$prj)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory (src)
add_subdirectory (chk)

add_custom_target (BuildApp ALL DEPENDS $prj)
EOF
cp $base/cmake/modules/CodeCoverage.cmake cmake/modules
cp $base/cmake/modules/FindCppUnit.cmake cmake/modules
fi

if ! [ -r src/CMakeLists.txt ]; then
cat > src/CMakeLists.txt <<EOF
# vim:set ts=4 sw=4 noexpandtab:

include_directories (
        \${Boost_INCLUDE_DIRS}
)

add_library ($prj SHARED
        Logger.cpp
)

target_link_libraries ($prj
        \${Boost_LIBRARIES}
        \${CMAKE_THREAD_LIBS_INIT}
)

add_executable (${prj}cli ${prj}cli.cpp)
target_link_libraries (${prj}cli $prj)
EOF
fi

if ! [ -r chk/CMakeLists.txt ]; then
cat > chk/CMakeLists.txt <<EOF
# vim:set ts=4 sw=4 noexpandtab:

find_package (CppUnit REQUIRED)

include_directories (
	\${CPPUNIT_INCLUDE_DIR}
	\${CMAKE_SOURCE_DIR}/src
)

add_executable (chk
   	chk.cpp
)

target_link_libraries (chk
	$prj
	\${CPPUNIT_LIBRARY}
)
EOF
fi
