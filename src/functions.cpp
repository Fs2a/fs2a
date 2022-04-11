/* BSD 3-Clause License
 *
 * Copyright (c) 2020, Fs2a, Simon de Hartog <simon@fs2a.pro>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * vim:set ts=4 sw=4 noet tw=120: */

#include "functions.h"

namespace Fs2a {

	const std::unordered_map<std::string, const char> hcerc2asc = {
		{ "amp",   '&' },
		{ "gt",    '>' },
		{ "lt",    '<' },
		{ "nbsp",  ' ' },
		{ "plus",  '+' },
		{ "quest", '?' }
	};

	std::string htmlCharEntRefCodes2ascii(const std::string & str_i)
	{
		std::string rv = str_i;
		std::string hcerc; // HTML Character Entity Reference Code
		// Locations of ampersand (&) and SemiColon (;)
		size_t amp = 0, sc = 0;
		static size_t maxlen = 0;
		if (maxlen == 0) {
			for (const auto & p : hcerc2asc) {
				if (p.first.size() > maxlen) maxlen = p.first.size();
			}
		}

		while ( (amp = rv.find('&', amp)) != std::string::npos) {
			sc = rv.find(';', amp);
			if (sc == std::string::npos) break; // We're done here
			if (sc - amp - 1 > maxlen) {
				// HTMl Character Entity Reference Code to big, continue
				amp++;
				continue;
			}
			hcerc = rv.substr(amp+1, sc - amp - 1);
			const auto rci = hcerc2asc.find(hcerc);
			if (rci == hcerc2asc.end()) {
				// Unknown HTML Character Entity Reference Code, skip replacement and leave as is
				amp = sc + 1;
				continue;
			}
			rv.replace(amp, sc - amp + 1, 1, rci->second);
			amp++;
		}

		return rv;
	}

} // Fs2a namespace
