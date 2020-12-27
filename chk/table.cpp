/* BSD 3-Clause License
 *
 * Copyright (c) 2020, Fs2a, Simon de Hartog <simon@fs2a.pro>
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * vim:set ts=4 sw=4 noet: */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Table.h>

#define CHECKNAME tableCheck

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CHECKNAME);
	CPPUNIT_TEST(construction);
	CPPUNIT_TEST(columns);
	CPPUNIT_TEST(rows);
	CPPUNIT_TEST_SUITE_END();

	public:

	CHECKNAME()
	{ }

	void setUp() {
	}

	void tearDown() {
	}

	void construction() {
		Fs2a::Table<int> t;

		// Check that column count is zero
		CPPUNIT_ASSERT_EQUAL(0, t.columns());

		// Check that row count is zero
		CPPUNIT_ASSERT_EQUAL(0, t.rows());

		// Check that all methods throw exceptions when no column count has been set yet
		CPPUNIT_ASSERT_THROW(t.rows(0), std::logic_error);
		CPPUNIT_ASSERT_THROW(t.rows(3), std::logic_error);
		CPPUNIT_ASSERT_THROW(t.cell(0, 0), std::logic_error);
		CPPUNIT_ASSERT_THROW(t.cell(8, 0), std::logic_error);
		CPPUNIT_ASSERT_THROW(t.cell(0, 5), std::logic_error);
		CPPUNIT_ASSERT_THROW(t.cell(4, 7), std::logic_error);
	}

	void columns() {
		Fs2a::Table<int> t;

		// Zero columns not allowed
		CPPUNIT_ASSERT_THROW(t.columns(0), std::invalid_argument);

		// Column count remains zero after throw
		CPPUNIT_ASSERT_EQUAL(0, t.columns());

		// Regular column set
		CPPUNIT_ASSERT_NO_THROW(t.columns(2));

		// Check that column count was set
		CPPUNIT_ASSERT_EQUAL(2, t.columns());

		// Not allowed to change columns
		CPPUNIT_ASSERT_THROW(t.columns(3), std::logic_error);

		// Column count should not have changed
		CPPUNIT_ASSERT_EQUAL(2, t.columns());

		// Can re-set to same number of columns
		CPPUNIT_ASSERT_NO_THROW(t.columns(2));

		// Column count should not have changed
		CPPUNIT_ASSERT_EQUAL(2, t.columns());
	}

	void rows() {
		Fs2a::Table<int> t;

		CPPUNIT_ASSERT_NO_THROW(t.columns(2));

		// Assure row count is still zero after column set
		CPPUNIT_ASSERT_EQUAL(0, t.rows());

};

#undef CHECKNAME
