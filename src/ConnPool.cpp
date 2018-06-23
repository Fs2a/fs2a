/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include <algorithm>
#include <cstring>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <signal.h>
#include "ConnPool.h"
#include "../globals.h"
#include "../Logger.h"

namespace Fs2a {

	ConnPool::ConnPool()
		: checking_a(false)
	{
		// Ignore SIGPIPE, otherwise it terminates our application when a
		// connection can't be established or is broken
		signal(SIGPIPE, SIG_IGN);
	}

	ConnPool::~ConnPool()
	{
		purge();
	}

	ConnPool::dbc_t ConnPool::get(const std::string & params_i)
	{
		dbc_t con;
		uint64_t stid = simpleThreadId();

		std::lock_guard<std::mutex> lck(mux_a);

		pool_t::iterator i = pool_a.find(params_i);

		if (i == pool_a.end()) {
			/** Create new connection. This throws a
			 * pqxx::broken_connection exception when the connect fails.
			 * This is ok to be propagated to the caller. */
			if (checking_a) {
				con.reset(new pqxx::nullconnection(params_i));
			} else {
				con.reset(new pqxx::connection(params_i));
			}
			connmap_t cm;
			cm[stid] = con;
			pool_a[params_i] = cm;
			return con;
		}

		// Thread is already using connection
		connmap_t::iterator j = i->second.find(stid);

		if (j != i->second.end()) return j->second;

		// Thread has not used a connection recently, find free one
		for (j = i->second.begin(); j != i->second.end(); j++) {
			if (j->second.use_count() == 1) {
				// Connection is free
				con = j->second;
				i->second.erase(j->first);
				i->second[stid] = con;
				return con;
			}
		}

		// No free one found, create new
		if (checking_a) {
			con.reset(new pqxx::nullconnection(params_i));
		} else {
			con.reset(new pqxx::connection(params_i));
		}
		i->second[stid] = con;

		return con;
	}

	void ConnPool::purge(const std::string & params_i)
	{
		std::set<std::string> toErase;
		std::lock_guard<std::mutex> lck(mux_a);

		if (params_i == "") {
			for (auto & i : pool_a) {
				if (purge_(i.second)) toErase.insert(i.first);
			}

			for (auto & i : toErase) pool_a.erase(i);

			return;
		}

		pool_t::iterator i = pool_a.find(params_i);

		if (i == pool_a.end()) return;

		if (purge_(i->second)) pool_a.erase(i);
	}

	bool ConnPool::purge_(connmap_t & cm_i)
	{
		std::set<uint64_t> idles;

		for (auto & i : cm_i) {
			if (i.second.use_count() == 1) idles.insert(i.first);
		}

		for (auto & i : idles) cm_i.erase(i);

		return cm_i.size() == 0;
	}

} // Fs2a namespace
