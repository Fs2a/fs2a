/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018 (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include "run.h"

namespace Fs2a {

	uint8_t run(const std::string & command_i, const std::vector<std::string> & arguments_i)
	{
		if (!command_i.size()) throw std::logic_error("Command can't be empty");
		if (!arguments_i.size()) throw std::logic_error("At least one argument required as argv[0]");

		pid_t child = fork();
		char **argv;
		uint16_t i;
		int status;

		switch (child) {
			case -1:
				throw std::runtime_error("Failed to fork");

			case 0:
				// In the child
				argv = new char*[arguments_i.size() + 1];
				for (i = 0; i < arguments_i.size(); i++) {
					argv[i] = new char[arguments_i[i].size()+1];
					strcpy(argv[i], arguments_i[i].c_str());
				}
				argv[i] = NULL;
				execv(command_i.c_str(), argv);
				throw std::runtime_error("Failed execv(3)-ing the command");

			default:
				// Parent code below
				break;
		}

		pid_t ret = waitpid(child, &status, 0);
		if (ret != child) {
			throw std::runtime_error("Waitpid(2) returned incorrect PID");
		}
		if (!WIFEXITED(status)) {
			throw std::runtime_error("Child exited abnormally");
		}

		return WEXITSTATUS(status);
	}

} // Fs2a namespace
