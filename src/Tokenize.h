/** @author Simon de Hartog <simon@fs2a.pro> */

#pragma once

#include <string>
#include <vector>

void Tokenize(
	const std::string & str,
	std::vector<std::string> & tokens,
	const std::string & delimiters = " "
);
