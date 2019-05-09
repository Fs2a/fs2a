/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018 */

#pragma once

#include <atomic>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <syslog.h>
#include "Singleton.h"

/** @{ Logging macros for easy logging */

#ifndef NDEBUG
/// Log a Debug message
#define LD(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt, ##__VA_ARGS__) \
/// Log a Conditional Debug message
#define LCD(cond, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt, ##__VA_ARGS__); \
	}
/// Log a Conditional Debug message and do Action if condition does not hold
#define LCDA(cond, action, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt, ##__VA_ARGS__); \
		action; \
	}
/// Log a Conditional Debug message and Return if condition does not hold
#define LCDR(cond, ret, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt, ##__VA_ARGS__); \
		return ret; \
	}
/// Log a Conditional Debug message and Throw if condition does not hold
#define LCDT(cond, exc, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt, ##__VA_ARGS__); \
		throw exc; \
	}
#else
/// Debugging disabled
#define LD(fmt, ...) {}
#define LCD(cond, fmt, ...) \
	if (!(cond)) { }
#define LCDA(cond, action, fmt, ...) \
	if (!(cond)) { \
		action; \
	}
#define LCDR(cond, ret, fmt, ...) \
	if (!(cond)) { \
		return ret; \
	}
#define LCDT(cond, exc, fmt, ...) \
	if (!(cond)) { \
		throw exc; \
	}
#endif

/// Log an Informational message
#define LI(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt, ##__VA_ARGS__)
/// Log a Conditional Informational message
#define LCI(cond, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt, ##__VA_ARGS__); \
	}
/// Log a Conditional Informational message and do Action if condition does not hold
#define LCIA(cond, action, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt, ##__VA_ARGS__); \
		action; \
	}
/// Log a Conditional Informational message and Return if condition does not hold
#define LCIR(cond, ret, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt, ##__VA_ARGS__); \
		return ret; \
	}
/// Log a Conditional Informational message and Throw if condition does not hold
#define LCIT(cond, exc, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt, ##__VA_ARGS__); \
		throw exc; \
	}

/// Log a Warning message
#define LW(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt, ##__VA_ARGS__)
/// Log a Conditional Warning message
#define LCW(cond, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt, ##__VA_ARGS__); \
	}
/// Log a Conditional Warning message and do Action if condition does not hold
#define LCWA(cond, action, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt, ##__VA_ARGS__); \
		action; \
	}
/// Log a Conditional Warning message and Return if condition does not hold
#define LCWR(cond, ret, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt, ##__VA_ARGS__); \
		return ret; \
	}
/// Log a Conditional Warning message and Throw if condition does not hold
#define LCWT(cond, exc, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt, ##__VA_ARGS__); \
		throw exc; \
	}

/// Log an Error message
#define LE(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt, ##__VA_ARGS__)
/// Log a Conditional Error message
#define LCE(cond, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt, ##__VA_ARGS__); \
	}
/// Log a Conditional Error message and do Action if condition does not hold
#define LCEA(cond, action, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt, ##__VA_ARGS__); \
		action; \
	}
/// Log a Conditional Error message and Return if condition does not hold
#define LCER(cond, ret, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt, ##__VA_ARGS__); \
		return ret; \
	}
/// Log a Conditional Error message and Throw if condition does not hold
#define LCET(cond, exc, fmt, ...) \
	if (!(cond)) { \
		Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt, ##__VA_ARGS__); \
		throw exc; \
	}
/** @} */

class LoggerCheck;

namespace Fs2a {

	class Logger : public Fs2a::Singleton<Logger>
	{
			/// Singleton template as friend for construction
			friend class Fs2a::Singleton<Logger>;

			/// Check class is also a friend
			friend class ::LoggerCheck;

		private:
			/// Default constructor
			Logger();

			/// Copy constructor
			Logger(const Logger & obj_i) = delete;

			/// Assignment constructor
			Logger & operator=(const Logger & obj_i) = delete;

			/// Destructor
			~Logger();

		public:
			/// Definition of logging levels
			enum loglevel_t : uint8_t {
				none = LOG_CRIT,
				error = LOG_ERR,
				warning = LOG_WARNING,
				notice = LOG_NOTICE,
				info = LOG_INFO,
				debug = LOG_DEBUG
			};

		protected:
			/** Maintain a local string for syslog program identification,
			 * because openlog does not copy it. */
			std::string ident_a;

			/// Textual syslog levels map.
			std::map<loglevel_t, std::string> levels_a;

			/// Maximum log level to log.
			std::atomic<loglevel_t> maxlevel_a;

			/// Internal mutex to be MT safe
			std::mutex mymux_a;

			/// Characters to strip from beginning of filenames
			size_t strip_a;

			/// True when logging to syslog, false when logging to stderr
			bool syslog_a;

		public:

			/** Check whether the current logging destination is syslog.
			 * @returns True if logging to syslog, false if logging to
			 * stderr. */
			inline bool destSyslog() const {
				return syslog_a;
			}

			/** Log a formatted message based on the given parameters.
			 * Please use the convenience logging macros instead of this
			 * method.
			 * @param log_i Whether to log the formatted line as well
			 * @param file_i Filename we are logging from
			 * @param line_i Line number at which we are logging
			 * @param priority_i Syslog priority level
			 * @param fmt_i Format argument for remainder of arguments */
			void log(
				const std::string & file_i,
				const size_t & line_i,
				const loglevel_t priority_i,
				const char *fmt_i,
				...
			);

			/** Return the maximum log level which is logged.
			 * @returns Maximum log level. */
			inline loglevel_t maxlevel() const { return maxlevel_a; }

			/** Set the maximum log level to log.
			 * @param level_i New maximum log level. */
			inline void maxlevel(const loglevel_t level_i) { maxlevel_a = level_i; }

			/** Write all subsequent logs to stderr.
			 * @param strip_i Number of characters to strip from beginning of
			 * filenames to shorten log output, default 0 */
			void stderror(const size_t strip_i = 0);

			/** Write all following logs to syslog with specified program name.
			 * @param ident_i Program identification
			 * @param facility_i Syslog facility to use as specified in the
			 * syslog(3) manual page (man 3 syslog), default LOG_LOCAL0
			 * @param strip_i Number of characters to strip from beginning of
			 * filenames to short log output, default 0
			 * @returns True if succeeded, false if already opened. */
			bool syslog(const std::string ident_i, const int facility_i = LOG_LOCAL0, const size_t strip_i = 0);

	};

} // Fs2a namespace
