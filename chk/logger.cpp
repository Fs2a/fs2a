/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Logger.h>

#define CHECKNAME LoggerCheck

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(CHECKNAME);
		CPPUNIT_TEST(logdest);
		CPPUNIT_TEST_SUITE_END();

	public:
		void logdest()
		{
			Fs2a::Logger *l = nullptr;

			// Can we construct without issues?
			CPPUNIT_ASSERT_NO_THROW(l = Fs2a::Logger::instance());

			// Ensure first setting is stderr
			CPPUNIT_ASSERT_EQUAL(false, l->destSyslog());

			// Ok to reenable stderror
			CPPUNIT_ASSERT_NO_THROW(l->stderror());
			CPPUNIT_ASSERT_EQUAL(false, l->destSyslog());

			// Ensure syslog level mappings are in place
			CPPUNIT_ASSERT_EQUAL((size_t) 8, l->levels_a.size());

			// Can we open to syslog?
			CPPUNIT_ASSERT(l->syslog("LoggerCheck"));
			CPPUNIT_ASSERT(l->destSyslog());

			// Syslog open twice is a failure
			CPPUNIT_ASSERT_EQUAL(false, l->syslog("LoggerCheck"));
			CPPUNIT_ASSERT(l->destSyslog());

			// Back to stderror?
			CPPUNIT_ASSERT_NO_THROW(l->stderror());
			CPPUNIT_ASSERT_EQUAL(false, l->destSyslog());

			// And back to syslog should be OK
			CPPUNIT_ASSERT(l->syslog("LoggerCheck"));
			CPPUNIT_ASSERT(l->destSyslog());
		}

};
