/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2019
 * vim:set ts=4 sw=4 noexpandtab:
 * @description This is a header-only implementation in C++ for encoding
 * and decoding data to and from a base64 string without adding newlines. */

#pragma once

#include <cmath>
#include <cstring>
#include <memory>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Fs2a {

	/** Decode base64-encoded data back to its original.
	 * @param T the vector data type to return, can be either std::byte
	 * (C++17), unsigned char or char.
	 * @param b64_i The Base64-encoded data to be decoded.
	 * @returns Vector of std::byte's. Access the bytes directly via the
	 * data() member and the length via the size() member. */
	template <typename T>
	std::vector<T> base64decode(const std::string & b64_i) {
		static const char b64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		std::vector<T> data;
		uint8_t offset=0; // Offset in base64 decoding, either 0, 1, 2 or 3
		size_t i=0;
		T b = 0x00; // Byte to build up
		const char *p = nullptr;  // Pointer to found char in Base64 dictionary
		T v = 0x00; // Value from Base64 data
		std::ostringstream oss;

		while (i < b64_i.size()) {
			p = index(b64, b64_i[i]);
			if (p == nullptr) {
				switch (b64_i[i]) {
					case '\r':
					case '\n':
						i++; continue;

					case '\\':
						i++;
						if (i < b64_i.size() && b64_i[i] == 'n') {
							/* \n found meaning newline, skip it entirely.
							 * This is not according to standards, but it is
							 * something the Bunq Bank API uses. */
							i++;
						}
						continue;

					case '=':
						switch (offset) {
							case 0:
							case 1:
								throw std::runtime_error(
									"Equal sign encountered while decoding first or second character "
									"in base64 quad"
								);

							default:
								// No further bytes available
								break;
						}
						return data;

					default:
						break;
				}
				oss << "Unknown Base64 character \"" << b64_i[i];
				oss << "\" encountered at position " << i;
				throw std::runtime_error(oss.str());
			}
			v = p - b64;

			if (v & 0xC0) throw std::logic_error("Base64 character has index value larger than 64?");

			/* Here the actual Base64 decoding starts. We are building up new
			 * data bytes as Base64 comes in. Once a data byte is finished,
			 * push it onto the vector. This assures we always have the latest
			 * and complete data vector available for returning it. */
			switch (offset) {
				case 3:
					b |= (v & 0x3F);
					data.push_back(b);
					b = 0x00;
					break;

				case 2:
					b |= ((v >> 2) & 0x0F);
					data.push_back(b);
					b = (v << 6) & 0xC0;
					break;

				case 1:
					b |= ((v >> 4) & 0x03);
					data.push_back(b);
					b = (v << 4) & 0xF0;
					break;

				case 0:
					b = (v << 2) & 0xFC;
					break;

				default:
					throw std::logic_error("Internal Base64 quad offset is not 0, 1, 2 or 3");
			}

			offset = (offset + 1) % 4;
		}

		return data;
	}

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
