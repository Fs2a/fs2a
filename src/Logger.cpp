/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2017 */

#include <cstdarg>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "Logger.h"

namespace Fs2a {

	Logger::Logger()
		: opened_a(false), strip_a(0)
	{
	}

	Logger::~Logger()
	{
		std::lock_guard<std::mutex> lck(mymux_a);

		if (opened_a) {
			closelog();
			opened_a = false;
		}
	}

	void Logger::log(
		const std::string & file_i,
		const size_t & line_i,
		const int priority_i,
		const char *fmt_i,
		...
	)
	{
		va_list args;         // Variable arguments list
		char buf[BUFSIZ];     // Buffer to store log string
		size_t count = 0;     // Number of percent signs in fmt_i
		std::string fmt;      // Separate format string for counting
		int offset = 0;       // Offset to continue string printing
		std::ostringstream oss; // Output Stringstream to write Thread ID to
		size_t pos = 0;       // Position in string of character
		struct timeval tv;    // Time value storage
		struct tm timeParts;  // Different parts of current time

		if (fmt_i == nullptr) return;

		fmt.assign(fmt_i);

		gettimeofday(&tv, nullptr);
		gmtime_r(&(tv.tv_sec), &timeParts);
		oss << std::this_thread::get_id();

		offset = sprintf(
					 buf,
					 "%02d:%02d:%02d.%06ld [%s] %s:%ld ",
					 timeParts.tm_hour,
					 timeParts.tm_min,
					 timeParts.tm_sec,
					 tv.tv_usec,
					 oss.str().c_str(),
					 file_i.substr(strip_a).c_str(),
					 line_i
				 );

		// Remove double percent signs
		while ((pos = fmt.find("%%")) != std::string::npos) fmt.erase(pos, 2);

		// Now count remaining percent signs to know number of args
		pos = 0;

		while ((pos = fmt.find('%', pos)) != std::string::npos) {
			count++;
			pos++;
		}

		if (count > 0) {
			va_start(args, count);
			vsprintf(buf + offset, fmt_i, args);
			va_end(args);
		}
		else {
			strcpy(buf + offset, fmt_i);
		}

		syslog(priority_i, "%s", buf);

		return;
	}

	bool Logger::open(const std::string ident_i, const int facility_i, const size_t strip_i)
	{
		std::lock_guard<std::mutex> lck(mymux_a);

		if (opened_a) return false;

		ident_a = ident_i;
		strip_a = strip_i;

		openlog(ident_a.c_str(), LOG_CONS | LOG_NDELAY | LOG_PID, facility_i);
		opened_a = true;
		return true;
	}

} // Fs2a namespace
