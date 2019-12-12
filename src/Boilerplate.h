// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <boost/program_options.hpp>

#define STR(s) XSTR(s)
#define XSTR(s) #s

#define FS2A_PO namespace po = boost::program_options

#define FS2A OPTPARSE \
	po::variables_map vm; \
	po::store(po::parse_command_line(argc, argv,

FS2A_PO;

namespace Fs2a {

	/** Convenience class for boilerplate code related to startup, config
	 * parsing and initialization. */
	class Boilerplate
	{
		protected:

		public:
		/// Constructor
		Boilerplate() = default;

		/// Destructor
		~Boilerplate() = default;

		/** Create a basic Options Description object.
		 * @param title Title to use when displaying help.
		 * @returns an Options Description object */
		static po::options_description basicOptDesc(const std::string & title = "Allowed options:");

	};

} // Fs2a namespace
