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

#include <stdexcept>
#include <fs2a/Logger.h>
#include <fs2a/NaiveDate.h>

namespace Fs2a {

	NaiveDate::NaiveDate()
	: day_(0), month_(0), year_(0)
	{ }

		/** Get the day of the month.
		 * @returns Internally stored day of the month in the range between 1 and 31.
		 * @throws std::range_error If the internal day of the month is not valid. */
	uint8_t NaiveDate::day() const
	{
		FCET(day_ >= 1 && day_ <= 31, std::range_error,
			"Day of month not im the range between 1 and 31");
		return day_;
	}

		/** Set the day of the month in the range 1-31.
		 * @param day_i Day of the month to set.
		 * @throws std::invalid_argument If the given day is not valid. */
	void NaiveDate::day(const uint8_t day_i)
	{
		FCET(day_i >= 1 && day_i <= 31, std::invalid_argument,
			"Given day of month '{:d}' not in the range between 1 and 31", day_i);
		day_ = day_i;
	}

		/** Parse a date from ISO8601 format, i.e., YYYY-MM-DD.
		 * @param date_i Date in ISO8601 format, meaning YYYY-MM-DD.
		 * @throws std::invalid_argument When the date was not parsable or invalid. */
		void iso8601(const std::string & date_i);

		/** Return the internal date in the ISO8601 format, i.e., YYYY-MM-DD.
		 * @throws std::range_error when the internal date is not valid. */
		std::string iso8601() const;

		/** Is the internally stored year a leap year?
		 * @returns True if leap year, false if not.
		 * @throws std::range_error if year is invalid, i.e/, 0. */
		bool leap() const;

		/** Get the month.
		 * @throws std::range_error If the internal month is not valid. */
		uint8_t NaiveDate::month() const
		{
			FCET(month_ >= 1 && month_ <= 12, std::range_error,
				"Month not in the range between 1 and 12");
			return month_;
		}

		/** Set the month.
		 * @param month_i Month to set.
		 * @throws std::invalid_argument If the given month is not valid. */
		void NaiveDate::month(const uint8_t month_i)
			FCET(month_i >= 1 && month_i <= 12, std::invalid_argument,
				"Given month '{:d}' not in the range between 1 and 12", month_i);
			month_ = month_i;
		}

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
