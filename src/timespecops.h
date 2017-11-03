/** @author Simon de Hartog <simon@fs2a.eu> */

#pragma once

#include <iostream>
#include <time.h>

namespace std {

	bool operator<(const struct timespec & lhs, const struct timespec & rhs);
	bool operator<=(const struct timespec & lhs, const struct timespec & rhs);
	bool operator>(const struct timespec & lhs, const struct timespec & rhs);
	bool operator>=(const struct timespec & lhs, const struct timespec & rhs);
	bool operator==(const struct timespec & lhs, const struct timespec & rhs);
	bool operator!=(const struct timespec & lhs, const struct timespec & rhs);
	void operator-=(struct timespec & lhs, const struct timespec & rhs);
	struct timespec operator+(const struct timespec & lhs, const struct timespec & rhs);
	std::ostream & operator<<(std::ostream & os, const struct timespec & ts);

} // std namespace
