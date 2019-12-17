// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <memory>
#include <string>
#include <jsoncpp/json/json.h>

namespace Fs2a {

	inline void string2json(const std::string & str_i, Json::Value *json_i) {
		static std::unique_ptr<Json::CharReader> reader;
		std::string errors;

		if (!reader) {
			Json::CharReaderBuilder builder;
			reader.reset(builder.newCharReader());
		}

		if (!reader->parse(str_i.c_str(), str_i.c_str() + str_i.size(), json_i, &errors)) {
			throw runtime_exception(std::string("Json parsing failed: ") + errors);
		}
	}

} // Fs2a namespace
