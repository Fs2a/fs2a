/** @author   Bren de Hartog <bren@fs2a.pro>
 * @copyright Copyright (c) 2025, Bren de Hartog. All rights reserved.
 * @license   This project is licensed under 3-clause BSD license:
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#pragma once

#include <cstdint>
#include <string>

namespace Fs2a {

	/** Class that contains a naive date.
	 * It doesn't take timezones into account. Can be used for one-to-one transfer of dates in
	 * the case conversion to or from epoch timestamps is not required or can be assumed to be all
	 * in the same timezone. */
	class NaiveDate {
		protected:
		/// Day of the month, sentinel value is 0
		uint8_t day_;

		/// Month, sentinel value is 0
		uint8_t month_;

		/// Year of the date, sentinel value is 0
		uint16_t year_;

		public:
		/// Basic constructor
		NaiveDate();

		/// Default destructor
		~NaiveDate() = default;

		/** Get the day of the month.
		 * @returns Internally stored day of the month in the range between 1 and 31.
		 * @throws std::range_error If the internal day of the month is not valid. */
		uint8_t day() const;

		/** Set the day of the month in the range 1-31.
		 * @param day_i Day of the month to set.
		 * @throws std::invalid_argument If the given day is not valid. */
		void day(const uint8_t day);

		/** Parse a date from ISO8601 format, i.e., YYYY-MM-DD.
		 * @param date_i Date in ISO8601 format, meaning YYYY-MM-DD.
		 * @throws std::invalid_argument When the date was not parsable or invalid. */
		void iso8601(const std::string & date_i);

		/** Return the internal date in the ISO8601 format, i.e., YYYY-MM-DD.
		 * @throws std::range_error when the internal date is not valid. */
		std::string iso8601() const;

		/** Is the internally stored year a leap year?
		 * @returns True if leap year, false if not. */
		bool leap() const;

		/** Get the month.
		 * @throws std::range_error If the internal month is not valid. */
		uint8_t month() const;

		/** Set the month.
		 * @param month_i Month to set.
		 * @throws std::invalid_argument If the given month is not valid. */
		void month(const uint8_t month_i);

		/** Check whether the internally stored date is valid.
		 * This method takes leap years into account. */
		bool valid() const;

		/** Get the year.
		 * @throws std::range_error If the internal year is not valid. */
		uint16_t year() const;

		/** Set the year.
		 * @param year_i Year to set. Can be in the range between 1 and 65535.
		 * @throws std::invalid_argument If the given year is not valid. Not allowed to be 0. */
		void year(const uint16_t year_i);

	}; // NaiveDate class
} // Fs2a namespace
