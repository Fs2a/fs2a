// vim:set ts=4 sw=4 noexpandtab:

/** This header file contains common preprocessor definitations that are used
 * in other Fs2a code and can also serve new code. */

#pragma once

#include <mutex>

/** Shorthand for setting up a lock guard for a simple mutex. */
#define GRD(x) std::lock_guard<std::mutex> lckgrd(x)

/** Shorthand for setting up a lock guard for a recursive mutex. */
#define RGRD(x) std::lock_guard<std::recursive_mutex> lckgrd(x)

/** Prevent unused parameter warnings from the compiler. */
#define UNUSED(expr) (void)(expr)

