/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2017 */

#include <memory>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Singleton.h>

#define CHECKNAME SingletonCheck

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(CHECKNAME);
		CPPUNIT_TEST(instance);
		CPPUNIT_TEST_SUITE_END();

		class DummySingleton : public Fs2a::Singleton<DummySingleton> {
				friend class Fs2a::Singleton<DummySingleton>;

				DummySingleton()
					: dummy(true) {}

			protected:
				bool dummy;

			public:
				bool check()
				{
					return dummy;
				}
		};

	public:
		void instance()
		{
			DummySingleton *ds = nullptr;

			CPPUNIT_ASSERT_EQUAL(false, DummySingleton::is_constructed());
			CPPUNIT_ASSERT_NO_THROW(ds = DummySingleton::instance());
			CPPUNIT_ASSERT_EQUAL(true, DummySingleton::is_constructed());
			CPPUNIT_ASSERT(ds->check());
			CPPUNIT_ASSERT_NO_THROW(DummySingleton::close());
			CPPUNIT_ASSERT_EQUAL(false, DummySingleton::is_constructed());
		}

};
