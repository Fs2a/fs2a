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

#include <cstdint>
#include <string>
#include "CsvReader.h"

#define ERR "Error at line "s + std::to_string(row + 1) + ", character "s + std::to_string(pos) + ": "s

namespace Fs2a {

	Table<std::string> CvReader::read(std::istream & stream_i, const char separator_i)
	{
		using namespace std::string_literals;

		Table<std::string> t;
		std::vector<std::string> h; // Header
		bool readingHeader = true;
		bool quoted = false;
		char c;
		std::string f; // Field with data
		uint16_t col = 0;
		uint32_t row = 0;
		size_t line = 1;
		size_t pos = 1;
		std::ios::iostate oldmsk = stream_i.exceptions();

		enum last_e : uint8_t {
			beginQuote,
			endOfRecord,
			endQuote,
			fieldData,
			separator,
			startOfRecord
		};

		last_e last = startOfRecord;

		auto fieldReady = [&](const bool nxt) {
			if (readingHeader) {
				h.push_back(f);
				return;
			}
			t.cell(col, row) = f;
			if (nxt) {
				if (col >= t.columns()) {
					throw std::runtime_error(ERR + "More than "s + std::to_string(t.columns()) + " fields found in current record");
				}
				if (col == t.columns() - 1) {
					col = 0; row++;
				} else {
					col++;
				}
			}
		};

		// Disable exceptions for EOF on the stream, because we need to be able to handle EOF properly.
		// If something bad happens other than EOF, throw exception.
		stream_i.exceptions(istream::badbit);

		while (stream_i.good()) {
			stream_i.read(&c, 1);
			if (stream_i.eof()) c = EOF;
			else pos++;

			// Skip valid but unused 8-bit characters at start of CSV
			if (readingHeader && last == startOfRecord && col == 0 && row == 0 && c != EOF && c & 0x80) continue;

			// If a field is quoted, only check for EOF, escaped quotes or end quote.
			// Store everything else (including separators and newlines) in the field data.
			// Do check newlines to keep track of character position.
			if (quoted) {
				switch (c) {
					case EOF:
						throw std::runtime_error(ERR + "Encountered EOF while reading quoted field");

					case '\r':
						if (stream_i.peek() == '\n') {
							f.append(c);
							// Swallow \r\n
							stream_i.read(&c, 1);
						}
						// Fallthrough
					case '\n':
						f.append(c);
						line++; pos = 0;
						last = fieldData;
						continue;

					case '"':
						if (stream_i.peek() == '"') {
							// Escaped quote
							stream_i.read(&c, 1);
							pos++;
							f.append(c);
							last = fieldData;
						} else {
							// End quote
							fieldReady(false);
							quoted = false;
							last = endQuote;
						}
						continue;

					default:
						f.append(c);
						last = fieldData;
						continue;
				}
			}

			// From here on, we are not reading a quoted field, so parse normally
			if (c == '\r' || c == '\n' || c == EOF) {
				// End of record
				if (c == '\r' && stream_i.peek() == '\n') {
					// Swallow \r\n
					stream_i.read(&c, 1);
					pos++;
				}
				switch (last) {
					case beginQuote:
						throw std::runtime_error(ERR + "Found End Of Record just after begin quote");

					case endOfRecord:
						throw std::logic_error(ERR + "Found EndOfRecord twice");

					case endQuote:
						break;

					case fieldData:
					case separator:
						fieldReady(false);
						break;

					case startOfRecord:
						if (stream_i.peek() != EOF) {
							throw std::runtime_error(ERR + "Found empty line");
						}
						break;
				}

				if (readingHeader) {
					if (h.empty()) {
						throw std::runtime_error(ERR + "Empty line at beginning, unable to determine number of columns"s);
					}
					// Both column adressing and count are
					// uint16_t, so max is UINT16_MAX - 1
					if (h.size() >= UINT16_MAX) {
						throw std::out_of_range(ERR + "Encountered too many columns "s + std::to_string(h.size()) +
							", maximum is " + std::to_string(UINT16_MAX-1));
					}
					t.columns(static_cast<uint16_t>(h.size()));
					for (col = 0; col < h.size(); col++) {
						t.cell(col, 0) = h.at(col);
					}
					h.clear();
					row = 1;
					col = 0;
					pos = 0;
					readingHeader = false;
				} else {
					if (col != t.columns()) {
						throw std::runtime_error("Line "s + std::to_string(row + 1) + " contains "s +
							std::to_string(col) + " fields, but column count is "s + std::to_string(t.columns()));
					}
					col = 0;
					if (row == UINT32_MAX) {
						throw std::runtime_error("Input data has more than "s + std::to_string(UINT32_MAX) + " rows");
					}
					row++;
				}
				pos = 0;
				continue;

			}

			switch (last) {
				case startOfRecord:
					f.clear();
					if (c == separator_i) {
						fieldReady(true);
						continue;
					}
					if (c == '"') {
						quoted = true;
						last = beginQuote;
						continue;
					}
					quoted = false;
					f.append(c);
					continue;

				case startOfField:
					switch (c) {
						case '"':
							quoted = true;
							e = readField;
							continue;

						case separator_i:
							// Empty field
							fieldReady(true);
							continue;

						default:
							quoted = false;
							f.append(c);
							e = readField;
							continue;
					}
					break;

				case readField:
					switch (c) {
						case '"':
							if (!quoted) {
								f.append(c);
								continue;
							}
							// Quoted field
							if (stream_i.peek() == c) {
								// Quoted quote, extract and add to field
								stream_i.read(&c, 1);
								pos++;
								f.append(c);
								continue;
							}
							// Single quote, end of field
							if (readingHeader) {
								h.push_back(f);
								e = separator;
								continue;
							}
							t.cell(col, row) = f;
							if (col >= t.columns() - 1) {
								e = endOfRecord;
							} else {
								e = separator;
							}
							continue;

						case separator_i:

						default:
							f.append(c);
							continue;
					}
					continue;
			}

		}

		stream_i.exceptions(oldmsk);
		return t;
	}

} // Fs2a namespace
