# BSD 3-Clause License
#
# Copyright (c) 2025, Fs2a, Bren de Hartog <bren@fs2a.pro>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required (VERSION 3.30)
if(POLICY CMP0015)
	cmake_policy(SET CMP0015 NEW)
endif()
if(POLICY CMP0056)
	cmake_policy(SET CMP0056 NEW)
endif()
if(POLICY CMP0167)
	cmake_policy(SET CMP0167 NEW)
endif()

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

project (fs2a)

# Custom C/C++ flags
set (CUSTOM_FLAGS   "-Wall -Werror -Wextra -Wno-varargs")
set (CUSTOM_DEBUG   "-O0 -g3 -ggdb")
set (CUSTOM_RELEASE "-O3")

# Common linker flags
set (CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} -s")

# Plain C flags
set (CMAKE_C_FLAGS         "${CUSTOM_FLAGS}")
set (CMAKE_C_FLAGS_DEBUG   "${CUSTOM_DEBUG}")
set (CMAKE_C_FLAGS_RELEASE "${CUSTOM_RELEASE}")

# C++ flags, mostly the same as C
set (CMAKE_CXX_FLAGS         "${CUSTOM_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG   "${CUSTOM_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE "${CUSTOM_RELEASE}")

# Find necessary packages
find_package (Boost REQUIRED COMPONENTS program_options system)
find_package (fmt REQUIRED)
find_package (Threads REQUIRED)

add_subdirectory (chk)
add_subdirectory (src)
