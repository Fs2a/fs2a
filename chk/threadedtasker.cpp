/* BSD 3-Clause License
 *
 * Copyright (c) 2024, Fs2a, Bren de Hartog <bren@fs2a.pro>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <memory>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <ThreadedTasker.h>

#define CHECKNAME threadedTaskerCheck

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CHECKNAME);
	CPPUNIT_TEST(testIdempotence);
	CPPUNIT_TEST_SUITE_END();

	public:
	void testIdempotence() {
		Fs2a::ThreadedTasker<> tt;

		/// Initial state after construction should be pristine and empty
		CPPUNIT_ASSERT_EQUAL(Fs2a::ThreadedTasker<>::states::pristine, tt.state());
		CPPUNIT_ASSERT_EQUAL(0UL, tt.size());

		/// Reset should not fail
		CPPUNIT_ASSERT_NO_THROW(tt.reset());
		CPPUNIT_ASSERT_EQUAL(Fs2a::ThreadedTasker<>::states::pristine, tt.state());
		CPPUNIT_ASSERT_EQUAL(0UL, tt.size());

		/// Abrupt termination should be OK
		CPPUNIT_ASSERT_NO_THROW(tt.stop());
		CPPUNIT_ASSERT_EQUAL(Fs2a::ThreadedTasker<>::states::stopped, tt.state());
		CPPUNIT_ASSERT_EQUAL(0UL, tt.size());

		/// Reset after terminate should work
		CPPUNIT_ASSERT_NO_THROW(tt.reset());
		CPPUNIT_ASSERT_EQUAL(Fs2a::ThreadedTasker<>::states::pristine, tt.state());
		CPPUNIT_ASSERT_EQUAL(0UL, tt.size());
	}

};
