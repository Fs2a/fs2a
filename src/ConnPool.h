/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <thread>
#include <pqxx/pqxx>
#include "Singleton.h"

/// Forward checkclass declaration for friendships
class ConnPoolCheck;

namespace Fs2a {

	/** Database connection pool Singleton that manages all DB connections.
	 */
	class ConnPool : public Fs2a::Singleton<ConnPool>
	{
			/// Check class can look inside data structures
			friend class ::ConnPoolCheck;

			/// Follow the Singleton pattern
			friend class Fs2a::Singleton<ConnPool>;

		private:
			/// Default constructor
			ConnPool();

			/// Copy constructor
			ConnPool(const ConnPool & obj_i) = delete;

			/// Assignment constructor
			ConnPool & operator=(const ConnPool & obj_i) = delete;

			/// Destructor
			~ConnPool();

		protected:
			/** Flag to determine whether we are simply checking code or
			 * not. If so, use null connections. */
			bool checking_a;

			/** Type definition for a map containing thread IDs to shared
			 * pointers with PostgreSQL connections. */
			typedef std::map <
			uint64_t,
			std::shared_ptr<pqxx::connection>
			> connmap_t;

			/** Type definition for internal connection pool administration.
			 * The map has a string as key, containing the PostgreSQL
			 * connection string. Only a literal string comparison is matched,
			 * permutations of settings are not taken into account. */
			typedef std::map <
			const std::string,
				  connmap_t
				  > pool_t;

			/// Internal mutex to control pool access
			std::mutex mux_a;

			/// Actual connection pool
			pool_t pool_a;

			/** Actively close idle connections within a specific pool.
			 * @param cm_i Connection map to purge of idle connections
			 * @returns True when specified map is empty after purge,
			 * false if not. */
			bool purge_(connmap_t & cm_i);

		public:
			/** Set checking on or off. When on, a null connection is used.
			 * @param checking_i True when checking, false when not. */
			void checking(const bool checking_i);

			/** Shorthand type definition for DataBaseConnection. */
			typedef std::shared_ptr<pqxx::connection> dbc_t;

			/** Get a database connection using @p params_i as connection
			 * string.
			 * @param params_i A PostgreSQL connection string. Each connection
			 * string comes with its own connection pool.
			 * @throws A runtime exception when the connection setup failed.
			 * @returns A PostgreSQL connection inside a std::shared_ptr */
			dbc_t get(const std::string & params_i);

			/** Get a database connection using @p params_i as connection
			 * string.
			 * @param params_i A PostgreSQL connection string. Each connection
			 * string comes with its own connection pool.
			 * @throws A runtime exception when the connection setup failed.
			 * @returns A PostgreSQL connection inside a std::shared_ptr */
			inline dbc_t get(const char *params_i) {
				return get(std::string(params_i));
			}

			/** Actively close all currently idle connections in the pool.
			 * @param params_i Unique connection string identifying which pool
			 * to purge. Can be the empty string (also the default) to purge
			 * all pools of idle connections. */
			void purge(const std::string & params_i = "");

	};

} // Fs2a namespace
