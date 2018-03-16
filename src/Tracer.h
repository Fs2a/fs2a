/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include <string>

#ifndef NDEBUG
#define TR Fs2a::Tracer tracerObject(__PRETTY_FUNCTION__)
#else
#define TR {}
#endif

namespace Fs2a {

	class Tracer
	{
		protected:
			std::string pf; // Pretty Function

		public:
			Tracer(const char *pf_i);
			~Tracer();
	};

} // Fs2a namespace
