/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018 (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

namespace Fs2a {

	/** Run a command via exec instead of via system(2).
	 * @param executable_i Executable to run with full path
	 * @param arguments_i Vector of arguments to use. This includes argument 0,
	 * so make sure you use the command in here.
	 * @returns The exit status of the command. */
	uint8_t run(const std::string & command_i, const std::vector<std::string> & arguments_i);

} // Fs2a namespace
