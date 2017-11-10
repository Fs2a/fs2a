/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2017 */

#pragma once

#include <syslog.h>
#include "Singleton.h"

/** @{ Logging macros for easy logging */

/// Obtain an output stream for logging a debug message
#define LD(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, LOG_DEBUG, fmt, ##__VA_ARGS__)

/// Obtain an output stream for logging an informational message
#define LI(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, LOG_INFO, fmt, ##__VA_ARGS__)

/// Obtain an output stream for logging a warning message
#define LW(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, LOG_WARNING, fmt, ##__VA_ARGS__)

/// Obtain an output stream for logging an error message
#define LE(fmt, ...) \
	Fs2a::Logger::instance()->log(__FILE__, __LINE__, LOG_ERR, fmt, ##__VA_ARGS__)
/** @} */

namespace Fs2a {

	class Logger : public Fs2a::Singleton<Logger> {
			/// Singleton template as friend for construction
			friend class Fs2a::Singleton<Logger>;

		private:
			/// Default constructor
			Logger();

			/// Copy constructor
			Logger(const Logger & obj_i) = delete;

			/// Assignment constructor
			Logger & operator=(const Logger & obj_i) = delete;

			/// Destructor
			~Logger();

		protected:
			/// Flag to keep track of whether syslog has been opened yet
			bool opened_a;

			/// Characters to strip from beginning of filenames
			size_t strip_a;

		public:
			/** Convenience syslogging function. Please call the appropriate logging
			 * macros instead of this method.
			 * @param file_i Filename we are logging from
			 * @param line_i Line number at which we are logging
			 * @param priority_i Syslog priority level
			 * @param fmt_i Format argument for remainder of arguments */
			void log(
				const std::string & file_i,
				const size_t & line_i,
				const int priority_i,
				const char *fmt_i,
				...
			);

			/** Initialize logging with specified program name.
			 * @param ident_i Program identification
			 * @param facility_i Syslog facility to use as specified in the
			 * syslog(3) manual page (man 3 syslog), default LOG_LOCAL0
			 * @param strip_i Number of characters to strip from beginning of
			 * filenames to short log output, default 0
			 * @returns True if succeeded, false if already opened. */
			bool open(const std::string ident_i, const int facility_i = LOG_LOCAL0, const size_t strip_i = 0);

	};

} // Fs2a namespace
