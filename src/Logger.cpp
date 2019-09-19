/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "Logger.h"

namespace Fs2a {

	Logger::Logger()
		: maxlevel_a(Logger::debug), strip_a(0), syslog_a(false)
	{
		levels_a[error]   = "ERROR";
		levels_a[warning] = "WARNING";
		levels_a[notice]  = "NOTICE";
		levels_a[info]    = "INFO";
		levels_a[debug]   = "DEBUG";
	}

	Logger::~Logger()
	{
		GRD(mymux_a);

		if (syslog_a) {
			closelog();
			syslog_a = false;
		}
	}

	void Logger::log(
		const std::string & file_i,
		const size_t & line_i,
		const loglevel_t priority_i,
		const char *fmt_i,
		...
	)
	{
		va_list args;           // Variable arguments list
		char buf[BUFSIZ];       // Char buffer for doing vsnprintf
		size_t count = 0;       // Number of percent signs in fmt_i
		std::string fmt;        // Separate format string for counting
		std::ostringstream oss; // Output Stringstream to write log string to
		size_t pos = 0;         // Position in string of character
		struct timeval tv;      // Time value storage
		struct tm timeParts;    // Different parts of current time

		if (fmt_i == nullptr) return;

		if (priority_i > maxlevel_a) return;

		fmt.assign(fmt_i);

		gettimeofday(&tv, nullptr);
		gmtime_r(&(tv.tv_sec), &timeParts);

		oss << std::setfill('0') << std::setw(2) << timeParts.tm_hour << ':';
		oss << std::setfill('0') << std::setw(2) << timeParts.tm_min << ':';
		oss << std::setfill('0') << std::setw(2) << timeParts.tm_sec << '.';
		oss << std::setfill('0') << std::setw(6) << tv.tv_usec;
		oss << " [" << std::this_thread::get_id() << "] ";
		oss << file_i.substr(strip_a) << ':';
		oss << std::setw(0) << line_i << ' ';

		if (!syslog_a) oss << levels_a[priority_i] << ' ';

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
			size_t rv = vsnprintf(buf, BUFSIZ, fmt_i, args);
			va_end(args);
			oss << buf;
			if (rv >= BUFSIZ) oss << " (truncated)";
		} else oss << fmt_i;

		if (syslog_a) {
			::syslog(priority_i, "%s", oss.str().c_str());
		} else {
			std::cerr << oss.str() << std::endl;
		}
	}

	void Logger::stderror(const size_t strip_i)
	{
		GRD(mymux_a);

		if (syslog_a) {
			closelog();
			syslog_a = false;
		}

		strip_a = strip_i;
	}

	bool Logger::syslog(const std::string ident_i, const int facility_i, const size_t strip_i)
	{
		GRD(mymux_a);

		if (syslog_a) return false;

		ident_a = ident_i;
		strip_a = strip_i;

		openlog(ident_a.c_str(), LOG_CONS | LOG_NDELAY | LOG_PID, facility_i);
		syslog_a = true;
		return true;
	}

} // Fs2a namespace
