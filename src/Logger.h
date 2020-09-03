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

#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <syslog.h>
#include <fmt/format.h>
#include "commondefs.h"
#include "Singleton.h"

/** @{ Logging macros for easy logging using old-school *printf %
 * replacements. */

#ifndef NDEBUG
/// Log a Debug message
#define LD(fmtstr, ...) \
{ \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, logbuf); \
}

/// Log a Conditional Debug message
#define LCD(cond, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, logbuf); \
}

/// Log a Conditional Debug message and do Action if condition does not hold
#define LCDA(cond, action, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, logbuf); \
	action; \
}

/// Log a Conditional Debug message and Return if condition does not hold
#define LCDR(cond, ret, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, logbuf); \
	return ret; \
}

#else /// Debugging disabled

#define LD(fmtstr, ...) {}
#define LCD(cond, fmtstr, ...) if (!(cond)) { }
#define LCDA(cond, action, fmtstr, ...) if (!(cond)) { action; }
#define LCDR(cond, ret, fmtstr, ...) if (!(cond)) { return ret; }
#endif

/// Log an Informational message
#define LI(fmtstr, ...) \
{ \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, logbuf); \
}

/// Log a Conditional Informational message
#define LCI(cond, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, logbuf); \
}

/// Log a Conditional Informational message and do Action if condition does not hold
#define LCIA(cond, action, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, logbuf); \
	action; \
}

/// Log a Conditional Informational message and Return if condition does not hold
#define LCIR(cond, ret, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, logbuf); \
	return ret; \
}

/// Log a Notice message
#define LN(fmtstr, ...) \
{ \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, logbuf); \
}

/// Log a Conditional Notice message
#define LCN(cond, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, logbuf); \
}

/// Log a Conditional Notice message and do Action if condition does not hold
#define LCNA(cond, action, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, logbuf); \
	action; \
}

/* Throw and Return variants are deprecated and only defined with other loglevels for backward compatibility.
* Since the Notice level has not been used before, there is no need for backward compatibility for
* this loglevel. */

/// Log a Warning message
#define LW(fmtstr, ...) \
{ \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, logbuf); \
}

/// Log a Conditional Warning message
#define LCW(cond, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, logbuf); \
}

/// Log a Conditional Warning message and do Action if condition does not hold
#define LCWA(cond, action, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, logbuf); \
	action; \
}

/// Log a Conditional Warning message and Return if condition does not hold
#define LCWR(cond, ret, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, logbuf); \
	return ret; \
}

/// Log a Conditional Warning message and Throw if condition does not hold
#define LCWT(cond, exc, fmtstr, ...) \
if (!(cond)) { \
	std::unique_ptr<std::string> logstr; \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	logstr = Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, logbuf); \
	throw exc(logstr->c_str()); \
}

/// Log an Error message
#define LE(fmtstr, ...) \
{ \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, logbuf); \
}

/// Log a Conditional Error message
#define LCE(cond, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, logbuf); \
}

/// Log a Conditional Error message and do Action if condition does not hold
#define LCEA(cond, action, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, logbuf); \
	action; \
}

/// Log a Conditional Error message and Return if condition does not hold
#define LCER(cond, ret, fmtstr, ...) \
if (!(cond)) { \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, logbuf); \
	return ret; \
}

/// Log a Conditional Error message and Throw if condition does not hold
#define LCET(cond, exc, fmtstr, ...) \
if (!(cond)) { \
	std::unique_ptr<std::string> logstr; \
	char logbuf[BUFSIZ]; \
	snprintf(logbuf, BUFSIZ, fmtstr, ##__VA_ARGS__); \
	logstr = Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, logbuf); \
	throw exc(logstr->c_str()); \
}
/** @} */

/** @{ Easy logging macros that use libFmt formatting. */
#ifndef NDEBUG
#define FD(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::debug, fmt::format(str, ##__VA_ARGS__))
#else
#define FD(str, ...) {}
#endif

#define FI(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt::format(str, ##__VA_ARGS__))
#define FCI(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt::format(str, ##__VA_ARGS__)); }
#define FCIA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::info, fmt::format(str, ##__VA_ARGS__)); \
	action; \
}

#define FN(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, fmt::format(str, ##__VA_ARGS__))
#define FCN(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, fmt::format(str, ##__VA_ARGS__)); }
#define FCNA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::notice, fmt::format(str, ##__VA_ARGS__)); \
	action; \
}

#define FW(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt::format(str, ##__VA_ARGS__))
#define FCW(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt::format(str, ##__VA_ARGS__)); }
#define FCWA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::warning, fmt::format(str, ##__VA_ARGS__)); \
	action; \
}

#define FE(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__))
#define FET(exc, str, ...) \
	throw exc(Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__))->c_str())
#define FCE(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__)); }
#define FCEA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__)); \
	action; \
}
#define FCER(cond, ret, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__)); \
	return ret; \
}
#define FCET(cond, exc, str, ...) if (!(cond)) { \
	throw exc(Fs2a::Logger::instance()->log(__FILE__, __LINE__, Fs2a::Logger::error, fmt::format(str, ##__VA_ARGS__))->c_str()); \
}


/** @} */

class LoggerCheck;

namespace Fs2a {

	class Logger : public Fs2a::Singleton<Logger> {
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
			std::string ident_;

			/// Textual syslog levels map.
			std::map<loglevel_t, std::string> levels_;

			/// Maximum log level to log.
			std::atomic<loglevel_t> maxlevel_;

			/// Internal mutex to be MT safe
			std::mutex mymux_;

			/// Stream to write to
			std::ostream * stream_;

			/// Characters to strip from beginning of filenames
			size_t strip_;

			/// True when logging to syslog, false when logging to stderr
			bool syslog_;

		public:

			/** Check whether the current logging destination is syslog.
			 * @returns True if logging to syslog, false if logging to
			 * stderr. */
			inline bool destSyslog() const
			{
				return syslog_;
			}

			/** Log a formatted message based on the given parameters.
			 * Please use the convenience logging macros instead of this
			 * method.
			 * @param file_i Filename we are logging from
			 * @param line_i Line number at which we are logging
			 * @param priority_i Syslog priority level
			 * @param msg_i Formatted message
			 * @returns Unique pointer to logged string. */
			std::unique_ptr<std::string> log(
				const std::string & file_i,
				const size_t & line_i,
				const loglevel_t priority_i,
				const std::string & msg_i
			);

			/** Return the maximum log level which is logged.
			 * @returns Maximum log level. */
			inline loglevel_t maxlevel() const
			{
				return maxlevel_;
			}

			/** Set the maximum log level to log.
			 * @param level_i New maximum log level. */
			inline void maxlevel(const loglevel_t level_i)
			{
				maxlevel_ = level_i;
			}

			/** Write all subsequent logs to stderr.
			 * @param strip_i Number of characters to strip from beginning of
			 * filenames to shorten log output, default 0 */
			inline void stderror(const size_t strip_i = 0) { stream(&std::cerr, strip_i); }

			/** Write all following logs to an output stream.
			 * @param stream_i Pointer to stream to write to, can be std::cout,
			 * std::cerr or any other output stream.
			 * @param strip_i Number of characters to strip from beginning of
			 * filenames to shorten log output, default 0.
			 * @throws std::invalid_argument when a null pointer is passed to
			 * @p stream_i. */
			void stream(std::ostream * stream_i, const size_t strip_i = 0);

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
