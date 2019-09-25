/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2019
 * vim:set ts=4 sw=4 noexpandtab:
 * @description This is a header-only implementation in C++ for encoding
 * data in to a base64 string without adding newlines. */

#pragma once

#include <cmath>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

namespace Fs2a {

	/** Encode a regular C string to a base64 std::string.
	 * @param T Choose either char or unsigned char, whatever suits you.
	 * @param string_i pointer to C string
	 * @returns The data encoded as Base64 string. */
	template <typename T>
	std::string base64encode(const T *string_i) {
		return Fs2a::base64encode<T>(string_i, strlen(string_i));
	}

	/** Encode data to a base64 std::string.
	 * @param T Choose either char or unsigned char, whatever suits you.
	 * @param data_i pointer to data
	 * @param len_i Length in bytes of data
	 * @returns The data encoded as Base64 string. */
	template <typename T>
	std::string base64encode(const T *data_i, const size_t len_i) {
		static const T b64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		std::string out="";
		size_t i=0;

		if (len_i == 0) return out;

		out.reserve((size_t) (ceil(len_i / 3.0) * 4));

		while (i+2 < len_i) {
			out += b64[(data_i[i] >> 2) & 0x3F];
			out += b64[((data_i[i] & 0x03) << 4) | ((data_i[i+1] >> 4) & 0x0F)];
			out += b64[((data_i[i+1] & 0x0F) << 2) | ((data_i[i+2] >> 6) & 0x03)];
			out += b64[data_i[i+2] & 0x3F];

			i+=3;
		}

		switch (len_i - i) {
			case 0:
				break;

			case 1:
				out += b64[(data_i[i] >> 2) & 0x3F];
				out += b64[((data_i[i] & 0x03) << 4)];
				out += '=';
				out += '=';
				break;

			case 2:
				out += b64[(data_i[i] >> 2) & 0x3F];
				out += b64[((data_i[i] & 0x03) << 4) | ((data_i[i+1] >> 4) & 0x0F)];
				out += b64[((data_i[i+1] & 0x0F) << 2)];
				out += '=';
				break;

			default:
				throw std::logic_error("Failure in length calculation, please contact author with input string");
		}

		return std::move(out);
	}

} // Fs2a namespace
