// vim:set ts=4 sw=4 noexpandtab:

#include <cstdint>
#include <stdexcept>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include "Application.h"
#include "IOsvcWrapper.h"
#include "Logger.h"

#define STR(s) XSTR(s)
#define XSTR(s) #s

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	int retval = 0;
	Fs2a::Logger *logger = nullptr;
	Fs2a::Application *app = nullptr;
	Fs2a::IOsvcWrapper * isw = nullptr;
	size_t strp = strlen(STR(GITREPOROOT))+1;

	try {
		po::options_description desc("This is a template daemon.\nThe default is to log to syslog using facility LOG_DAEMON. Command-line options:");

		desc.add_options()
			("help,h", "Show this help message")
			("debug,d", "Activate console logging to standard error instead of to syslog, implies -f")
			("foreground,f", "Do not daemonize, keep running in the foreground.")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			throw 0;
		}

		logger = Fs2a::Logger::instance();

		if (vm.count("debug")) {
			logger->stderror(strp);
			LD("Logging to standard error with strip length %d", strp);
		} else {
			logger->syslog("fs2atpld", LOG_DAEMON, strp);
			LD("Logging to syslog with strip length %d", strp);
		}
		if (!vm.count("debug") && !vm.count("foreground")) {
			LD("No debugging or foreground operation requested, daemonizing.");
			errno = 0;
			LCE(daemon(0, 0) == 0, "Failed to daemonize: %s", strerror(errno));
			errno = 0;
		}

		isw = Fs2a::IOsvcWrapper::instance();
		app = Fs2a::Application::instance();

		// Here comes the core code ;-)
		isw->stopWhenIdle(false);
		if (!app->init(&(isw->svc()))) throw 1;
		isw->runHere();

	} catch(const int & i) {
		retval = i;
	} catch(std::exception & se) {
		LE("General exception: %s", se.what());
		retval = 1;
	}

	if (app != nullptr) app->stop_listening();

	Fs2a::IOsvcWrapper::close();
	isw = nullptr;

	Fs2a::Singleton<Fs2a::Application>::close();
	app = nullptr;

	Fs2a::Logger::close();
	logger = nullptr;

	return retval;
}
