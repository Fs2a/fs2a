/** @author Simon de Hartog <simon@fs2a.eu> */

#include <iostream>
#include <iomanip>
#include <string.h>
#include "timespecops.h"

namespace std {

	bool operator<(const struct timespec & lhs, const struct timespec & rhs)
	{
		return (
				   lhs.tv_sec < rhs.tv_sec || (
					   lhs.tv_sec == rhs.tv_sec &&
					   lhs.tv_nsec < rhs.tv_nsec
				   )
			   );

	}

	bool operator<=(const struct timespec & lhs, const struct timespec & rhs)
	{
		return lhs == rhs || lhs < rhs;
	}

	bool operator>(const struct timespec & lhs, const struct timespec & rhs)
	{
		return ~(lhs <= rhs);
	}

	bool operator>=(const struct timespec & lhs, const struct timespec & rhs)
	{
		return ~(lhs < rhs);
	}

	bool operator==(const struct timespec & lhs, const struct timespec & rhs)
	{
		return lhs.tv_sec == rhs.tv_sec && lhs.tv_nsec == rhs.tv_nsec;
	}

	bool operator!=(const struct timespec & lhs, const struct timespec & rhs)
	{
		return ~(lhs == rhs);
	}

	void operator-=(struct timespec & lhs, const struct timespec & rhs)
	{
		lhs.tv_sec -= rhs.tv_sec;

		if (lhs.tv_nsec < rhs.tv_nsec) {
			lhs.tv_nsec = lhs.tv_nsec + 1000000000 - rhs.tv_nsec;
			lhs.tv_sec--;
		}
		else lhs.tv_nsec -= rhs.tv_nsec;
	}

	struct timespec operator+(const struct timespec & lhs, const struct timespec & rhs)
	{
		struct timespec rv;

		memcpy(&rv, &lhs, sizeof(struct timespec));
		rv.tv_sec += rhs.tv_sec;
		rv.tv_nsec += rhs.tv_nsec;

		if (rv.tv_nsec >= 1000000000) {
			rv.tv_nsec -= 1000000000;
			rv.tv_sec++;
		}

		return rv;
	}

	std::ostream & operator<<(std::ostream & os_i, const struct timespec & ts)
	{
		os_i << std::dec << ts.tv_sec << '.' << std::setw(9) << std::setfill('0') << ts.tv_nsec;
		return os_i;
	}

} // std namespace
