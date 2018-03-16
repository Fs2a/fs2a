/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

#include "Logger.h"
#include "Tracer.h"

namespace Fs2a {

	Tracer::Tracer(const char *pf_i)
	{
		if (pf_i == NULL) pf = "(No function)";
		else pf = pf_i;
		LD("%s entry", pf.c_str());
	}

	Tracer::~Tracer()
	{
		LD("%s exit", pf.c_str());
	}

} // Fs2a namespace
