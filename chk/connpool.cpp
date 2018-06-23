/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <Logger.h>
#include <Tracer.h>
#include <unistd.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <ConnPool.h>

#define CHECKNAME ConnPoolCheck
#define TEMPLATEDB "host=pgdb user=postgres dbname=template1"
#define POSTGRESDB "host=pgdb user=postgres dbname=postgres"

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

std::mutex conmux;
std::mutex termmux;
std::condition_variable concdv, termcdv;
bool connected, stop;

class CHECKNAME : public CppUnit::TestFixture
{
		CPPUNIT_TEST_SUITE(CHECKNAME);
		CPPUNIT_TEST(connect);
		CPPUNIT_TEST(purge);
		CPPUNIT_TEST(multi);
		CPPUNIT_TEST_SUITE_END();

	private:
		Fs2a::ConnPool<pqxx::nullconnection> *cp;
		std::unique_ptr<std::thread> t1, t2;

	public:
		uint64_t simpleThreadId(
                const std::thread::id & id_i = std::this_thread::get_id()
		) {
			std::stringstream ss;
			uint64_t stid;

			ss << id_i;
			ss >> stid;
			return stid;
        }

		void setUp() {
			CPPUNIT_ASSERT_EQUAL(false,
				Fs2a::ConnPool<pqxx::nullconnection>::is_constructed());
			cp = Fs2a::ConnPool<pqxx::nullconnection>::instance();
			CPPUNIT_ASSERT_EQUAL(true,
				Fs2a::ConnPool<pqxx::nullconnection>::is_constructed());
			connected = false;
			stop = false;
		}

		void tearDown() {
			stop = true;

			// Make sure all threads terminate
			termcdv.notify_all();

			// Join threads
			if (t1) t1->join();

			if (t2) t2->join();

			cp = nullptr;
			Fs2a::ConnPool<pqxx::nullconnection>::close();
			CPPUNIT_ASSERT_EQUAL(false,
				Fs2a::ConnPool<pqxx::nullconnection>::is_constructed());
		}

		static void connThread() {
			Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc =
				Fs2a::ConnPool<pqxx::nullconnection>::instance()->get(TEMPLATEDB);
			connected = true;
			concdv.notify_one();
			std::unique_lock<std::mutex> lck(termmux);
			termcdv.wait_for(
				lck,
				std::chrono::milliseconds(500),
				[] { return stop; }
			);
			CPPUNIT_ASSERT(stop);
		}

		void connect() {
			CPPUNIT_ASSERT_EQUAL((size_t) 0, cp->pool_a.size());
			CPPUNIT_ASSERT_EQUAL((size_t) 0, cp->pool_a.count(std::string(TEMPLATEDB)));
			Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc = cp->get(TEMPLATEDB);
			CPPUNIT_ASSERT(dbc.get() != nullptr);
			CPPUNIT_ASSERT_EQUAL(2L, dbc.use_count());
			CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
			CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.count(std::string(TEMPLATEDB)));

			Fs2a::ConnPool<pqxx::nullconnection>::pool_t::iterator i = cp->pool_a.begin();
			CPPUNIT_ASSERT(i != cp->pool_a.end());
			CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());
			Fs2a::ConnPool<pqxx::nullconnection>::connmap_t::iterator j = i->second.begin();
			CPPUNIT_ASSERT(j != i->second.end());
			CPPUNIT_ASSERT_EQUAL(dbc, j->second);
			CPPUNIT_ASSERT_EQUAL(simpleThreadId(), j->first);

			/** Check what happens when we call up a second connection from
			 * the same thread */
			{
				Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc2 = cp->get(TEMPLATEDB);
				CPPUNIT_ASSERT(dbc2.get() == dbc.get());
				CPPUNIT_ASSERT_EQUAL(3L, dbc.use_count());
				CPPUNIT_ASSERT_EQUAL(3L, dbc2.use_count());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.count(std::string(TEMPLATEDB)));

				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());
				j = i->second.begin();
				CPPUNIT_ASSERT(j != i->second.end());
				CPPUNIT_ASSERT_EQUAL(dbc, j->second);
				CPPUNIT_ASSERT_EQUAL(simpleThreadId(), j->first);
			}
			/// Scope ends for second connection dbc2, check stats
			CPPUNIT_ASSERT(dbc.get() != nullptr);
			CPPUNIT_ASSERT_EQUAL(2L, dbc.use_count());
		}

		void purge() {
			Fs2a::ConnPool<pqxx::nullconnection>::pool_t::iterator i;

			CPPUNIT_ASSERT_EQUAL((size_t) 0, cp->pool_a.size());

			// Purge on empty connection list succeeds
			CPPUNIT_ASSERT_NO_THROW(cp->purge());

			{
				// Acquire connection 1
				Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc = cp->get(TEMPLATEDB);
				CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
				i = cp->pool_a.find(std::string(TEMPLATEDB));
				CPPUNIT_ASSERT(i != cp->pool_a.end());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());

				// Purge with active connection does nothing
				CPPUNIT_ASSERT_NO_THROW(cp->purge());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
				i = cp->pool_a.find(std::string(TEMPLATEDB));
				CPPUNIT_ASSERT(i != cp->pool_a.end());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());
			}

			{
				// Acquire connection 2
				Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc2 = cp->get(POSTGRESDB);
				CPPUNIT_ASSERT_EQUAL((size_t) 2, cp->pool_a.size());
				i = cp->pool_a.find(std::string(POSTGRESDB));
				CPPUNIT_ASSERT(i != cp->pool_a.end());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());

				// Connection 1 is kept
				i = cp->pool_a.find(std::string(TEMPLATEDB));
				CPPUNIT_ASSERT(i != cp->pool_a.end());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());

				// Only purging busy conn 2 leaves other conn 1 alive
				CPPUNIT_ASSERT_NO_THROW(cp->purge(POSTGRESDB));
				i = cp->pool_a.find(std::string(TEMPLATEDB));
				CPPUNIT_ASSERT(i != cp->pool_a.end());
				CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());
			}

			// Purging connection 1 disconnects it ...
			CPPUNIT_ASSERT_NO_THROW(cp->purge(TEMPLATEDB));
			i = cp->pool_a.find(std::string(TEMPLATEDB));
			CPPUNIT_ASSERT(i == cp->pool_a.end());

			// and leaves other idle conn 2 intact
			i = cp->pool_a.find(std::string(POSTGRESDB));
			CPPUNIT_ASSERT(i != cp->pool_a.end());
			CPPUNIT_ASSERT_EQUAL((size_t) 1, i->second.size());

			// Purge with idle connection closes it
			CPPUNIT_ASSERT_NO_THROW(cp->purge());
			CPPUNIT_ASSERT_EQUAL((size_t) 0, cp->pool_a.size());
		}

		void multi() {
			Fs2a::ConnPool<pqxx::nullconnection>::pool_t::iterator i;
			uint64_t tid1, tid2;

			CPPUNIT_ASSERT_EQUAL((size_t) 0, cp->pool_a.size());

			Fs2a::ConnPool<pqxx::nullconnection>::dbc_t dbc0 = cp->get(TEMPLATEDB);

			{
				CPPUNIT_ASSERT_EQUAL(false, connected);
				CPPUNIT_ASSERT_EQUAL(false, stop);
				std::unique_lock<std::mutex> lck(conmux);
				t1.reset(new std::thread(CHECKNAME::connThread));
				tid1 = simpleThreadId(t1->get_id());

				// Wait until t1 is connected
				concdv.wait_for(
					lck,
					std::chrono::milliseconds(500),
					[] { return connected; }
				);
				CPPUNIT_ASSERT(connected);
			}

			CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
			i = cp->pool_a.find(std::string(TEMPLATEDB));
			CPPUNIT_ASSERT(i != cp->pool_a.end());
			CPPUNIT_ASSERT_EQUAL((size_t) 2, i->second.size());
			bool found = false;

			for (auto & m : i->second) {
				CPPUNIT_ASSERT(
					m.first == simpleThreadId() ||
					m.first == tid1
				);

				if (m.first == tid1) found = true;
			}

			CPPUNIT_ASSERT_EQUAL(true, found);

			// Terminate and remove T1
			stop = true;
			termcdv.notify_one();
			t1->join();
			t1.reset();

			// Create separate thread that reuses the thread ID from t1, so
			// t2 has a different thread ID
			stop = false;
			t1.reset(new std::thread([]() {
				std::unique_lock<std::mutex> lck(termmux);
				termcdv.wait_for(
					lck,
					std::chrono::milliseconds(500),
					[] { return stop; }
				);
			}));

			connected = false;
			// New thread, same conn, reuse from t1
			{
				std::unique_lock<std::mutex> lck(conmux);
				t2.reset(new std::thread(CHECKNAME::connThread));
				tid2 = simpleThreadId(t2->get_id());

				// Wait until t2 is connected
				concdv.wait_for(
					lck,
					std::chrono::milliseconds(500),
					[] { return connected; }
				);
				CPPUNIT_ASSERT(connected);
			}

			CPPUNIT_ASSERT(tid1 != tid2);
			CPPUNIT_ASSERT_EQUAL((size_t) 1, cp->pool_a.size());
			i = cp->pool_a.find(std::string(TEMPLATEDB));
			CPPUNIT_ASSERT(i != cp->pool_a.end());
			CPPUNIT_ASSERT_EQUAL((size_t) 2, i->second.size());
			found = false;
			bool bad = false;

			for (auto & m : i->second) {
				if (m.first == tid2) found = true;

				if (m.first == tid1) bad = true;

				CPPUNIT_ASSERT(
					m.first != tid1 && (
						m.first == simpleThreadId() ||
						m.first == tid2
					)
				);
			}

			CPPUNIT_ASSERT(found && !bad);
			stop = true;
			termcdv.notify_all();
		}

};
