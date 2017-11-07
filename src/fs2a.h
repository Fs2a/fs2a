/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2017 */

#pragma once

#define UNUSED(expr) (void)(expr)

#include <sys/time.h>
#include <boost/log/trivial.hpp>

/** Desired log format:
 * time_t with microseconds
 * thread ID
 * the file location relative to the repo basedir
 * A colon and a line number
 * Severity in CAPS
 * Rest of message */
// #define LD {BOOST_LOG_TRIVIAL(debug) << 
