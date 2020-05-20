/* Copyright (c) 2020 Simon de Hartog <simon@dehartog.name>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

vim:set ts=4 sw=4 noexpandtab: */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Observer.h>
#include <Observable.h>

#define CHECKNAME ObservingCheck

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CHECKNAME);
	CPPUNIT_TEST(all);
	CPPUNIT_TEST_SUITE_END();

	protected:
		class Watcher : public Fs2a::Observer<bool> {
			public:
				// Keep track whether we received an update
				bool updated;

				// Received value on update
				bool value;

				Watcher()
					: Fs2a::Observer<bool>::Observer(),
					updated(false), value(true)
				{ }

				void update(const bool & valueI) {
					updated = true;
					value = valueI;
				}
		};

		class Topic : public Fs2a::Observable<bool> {
			public:
				Topic(bool initI)
					: Fs2a::Observable<bool>::Observable(initI)
				{ }
		};

	public:
		void setUp() {
		}

		void tearDown() {
		}

		void all() {
			Topic t(false);
			Topic t2(true);

			// Check that the initialization value gets through. We do a
			// double test to assure that the compiler default value for
			// booleans does not accidentally make the test succeed.
			CPPUNIT_ASSERT_EQUAL(false, t.get());
			CPPUNIT_ASSERT_EQUAL(true,  t2.get());

			// Check that observable is not being observed by default
			CPPUNIT_ASSERT_EQUAL((size_t) 0, t.observers().size());

			// Check that setting same and different value without observer works
			CPPUNIT_ASSERT_NO_THROW(t.set(false));
			CPPUNIT_ASSERT_NO_THROW(t.set(true));

			// Separate scope for testing observer destructor
			{
				Watcher w;

				// Check that observer does not observe anything by
				// default.
				CPPUNIT_ASSERT_EQUAL(static_cast<Fs2a::Observable<bool> *>(nullptr), w.observable());

				// Check that an idle observer can ignore even more
				CPPUNIT_ASSERT_EQUAL(false, w.observe(nullptr));

				// Start observing t
				CPPUNIT_ASSERT_NO_THROW(w.observe(&t));

				// Check that watcher is observing t
				CPPUNIT_ASSERT_EQUAL(dynamic_cast<Fs2a::Observable<bool> *>(&t), w.observable());

				// Check that t is being observed by watcher
				CPPUNIT_ASSERT_EQUAL((size_t) 1, t.observers().size());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, t.observers().count(&w));

				// Check that watcher has not been updated yet
				CPPUNIT_ASSERT_EQUAL(false, w.updated);

				// Check that setting same value does not trigger update
				CPPUNIT_ASSERT_NO_THROW(t.set(true));
				CPPUNIT_ASSERT_EQUAL(false, w.updated);

				// Check that setting different value does trigger update
				CPPUNIT_ASSERT_NO_THROW(t.set(false));
				CPPUNIT_ASSERT_EQUAL(true, w.updated);
				CPPUNIT_ASSERT_EQUAL(false, w.value);
				w.updated = false;

				// Check that ignoring (unobserving) works
				CPPUNIT_ASSERT_EQUAL(true, w.observe(nullptr));
				CPPUNIT_ASSERT_EQUAL((size_t) 0, t.observers().size());
				CPPUNIT_ASSERT_EQUAL(false, w.observe(nullptr));
				CPPUNIT_ASSERT_NO_THROW(t.set(true));
				CPPUNIT_ASSERT_EQUAL(false, w.updated);

				// Observe again for destructor
				CPPUNIT_ASSERT_NO_THROW(w.observe(&t));
			}

			// Watcher destructed, check that it is ignoring t
			CPPUNIT_ASSERT_EQUAL((size_t) 0, t.observers().size());
		}

};
