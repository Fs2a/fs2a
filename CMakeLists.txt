# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd.

cmake_minimum_required (VERSION 2.8)
if(POLICY CMP0015)
  cmake_policy(SET CMP0015 NEW)
endif()
if(POLICY CMP0056)
  cmake_policy(SET CMP0056 NEW)
endif()

# To prevent output in GVim's compile window to be cluttered with wrongly
# interpreted escape sequences
set (CMAKE_COLOR_MAKEFILE=OFF)

project (fs2a)

# Custom C/C++ flags
set (CUSTOM_FLAGS   "-Wall -Werror -Wextra -pedantic-errors")
set (CUSTOM_DEBUG   "-O0 -g3 -ggdb")
set (CUSTOM_RELEASE "-O3")

# Common linker flags
set (CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} -s")

# Plain C flags
set (CMAKE_C_FLAGS         "${CUSTOM_FLAGS}")
set (CMAKE_C_FLAGS_DEBUG   "${CUSTOM_FLAGS} ${CUSTOM_DEBUG}")
set (CMAKE_C_FLAGS_RELEASE "${CUSTOM_FLAGS} ${CUSTOM_RELEASE}")

# C++ flags, mostly the same as C
set (CMAKE_CXX_FLAGS         "${CUSTOM_FLAGS} -std=gnu++11")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} ${CUSTOM_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CUSTOM_RELEASE}")

add_subdirectory (src)
add_subdirectory (chk)
