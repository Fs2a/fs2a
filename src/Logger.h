/* vim:set ts=4 sw=4 noet tw=90:
 *
 * BSD 3-Clause License
 *
 * Copyright © 2022, Bren de Hartog <bren@fs2a.pro>
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

#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <syslog.h>
#include <thread>
#include <fmt/format.h>
#include "commondefs.h"
#include "Singleton.h"

/** @{ Easy logging macros that use libFmt formatting. */
#ifndef NDEBUG

/** log a libFmt formatted Debug string */
#define FD(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::debug, fmt::format(FMT_STRING(str), ##__VA_ARGS__))

/** log a Conditional libFmt formatted Debug string */
#define FCD(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::debug, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); }

/** log a Conditional libFmt formatted Debug string and execute an additional Action when condition does not hold */
#define FCDA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::debug, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	action; \
}
#else
#define FD(str, ...) {}
#define FCD(cond, str, ...) if (!(cond)) {}
#define FCDA(cond, action, str, ...) if (!(cond)) { action; }
#endif

/** log a libFmt formatted Informational string */
#define FI(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::info, fmt::format(FMT_STRING(str), ##__VA_ARGS__))

/** log a Conditional libFmt formatted Informational string */
#define FCI(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::info, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); }

/** log a Conditional libFmt formatted Informational string and execute an additional Action when condition does not hold */
#define FCIA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::info, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	action; \
}

/** log a libFmt formatted Notification string */
#define FN(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::notice, fmt::format(FMT_STRING(str), ##__VA_ARGS__))

/** log a Conditional libFmt formatted Notification string */
#define FCN(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::notice, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); }

/** log a Conditional libFmt formatted Notification string and execute an additional Action when condition does not hold */
#define FCNA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::notice, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	action; \
}

/** log a libFmt formatted Warning string */
#define FW(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::warning, fmt::format(FMT_STRING(str), ##__VA_ARGS__))

/** log a Conditional libFmt formatted Notification string */
#define FCW(cond, str, ...) if (!(cond)) { Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::warning, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); }

/** log a Conditional libFmt formatted Warning string and execute an additional Action when condition does not hold */
#define FCWA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::warning, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	action; \
}

/** log a Conditional libFmt formatted Warning string and Return when condition does not hold */
#define FCWR(cond, ret, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::warning, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	return ret; \
}

/** log a libFmt formatted Error string */
#define FE(str, ...) Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__))

/** log a libFmt formatted Error string and Throw an exception with that same string */
#define FET(exc, str, ...) \
	throw exc(Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__))->c_str())

/** log a Conditional libFmt formatted Error string */
#define FCE(cond, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); }

/** log a Conditional libFmt formatted Error string and execute an additional Action when condition does not hold */
#define FCEA(cond, action, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	action; \
}

/** log a Conditional libFmt formatted Error string and Return when condition does not hold */
#define FCER(cond, ret, str, ...) if (!(cond)) { \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__)); \
	return ret; \
}

/** log a Conditional libFmt formatted Error string and Throw an exception with that same string */
#define FCET(cond, exc, str, ...) if (!(cond)) { \
	throw exc(Fs2a::Logger::instance()->log(__FILE__, __LINE__, \
		Fs2a::Logger::error, fmt::format(FMT_STRING(str), ##__VA_ARGS__))->c_str()); \
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

template <> struct fmt::formatter<std::thread::id> : formatter<string_view> {
	template <typename FormatContext>
	auto format(std::thread::id tid, FormatContext & ctx) const {
		std::ostringstream oss;
		oss << tid;
		return formatter<string_view>::format(oss.str(), ctx);
	}
};
