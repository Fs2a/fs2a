# vim:set ts=4 sw=4 noet tw=80:
#
# BSD 3-Clause License
#
# Copyright (c) 2022, Fs2a, Bren de Hartog <bren@fs2a.pro>
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

find_package (CppUnit REQUIRED)

add_executable (fs2achk
	chk.cpp
	functions.cpp
	logger.cpp
	observing.cpp
	readcsv.cpp
	singleton.cpp
	table.cpp
)

# Also ensure unitchecks are C++20 compliant
target_compile_features (fs2achk PUBLIC cxx_std_20)

target_include_directories (fs2achk
	PRIVATE
		${Boost_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/src
		${CPPUNIT_INCLUDE_DIR}
)

target_link_libraries (fs2achk
	${Boost_LIBRARIES}
	${CPPUNIT_LIBRARY}
	fs2a
)
