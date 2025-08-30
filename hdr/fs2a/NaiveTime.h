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

	/** Class to administer a time of day with hours and minutes.
	 * No seconds are taken into account and a 24-hour clock is used for representation. */
	class NaiveTime {
		/// Hour, sentinel value 0xFF
		uint8_t hour_;

		/// Minute, sentinel value 0xFF
		uint8_t min_;

		public:
		/// Default constructor
		NaiveTime();

		/// Default destructor
		~NaiveTime() = default;

		/** Get the internal hour.
		 * @returns hour in 24-hour format.
		 * @throws std::logic_error if internal hour is not yet initialized. */
		uint8_t hour() const;

		/** Set the hour.
		 * @param hour_i Hour to set in 24-hour format, so between 0 and 23.
		 * @throws std::invalid_argument if @p hour_i is greater than 23. */
		void hour(const uint8_t hour_i);

		/** Get the internal minute.
		 * @returns minute.
		 * @throws std::logic_error if internal minute is not yet initialized. */
		uint8_t minute() const;

		/** Set the minute.
		 * @param minute_i Minute to set. Should be between 0 and 59.
		 * @throws std::invalid_argument if @p minute_i is greater than 59. */
		void minute(const uint8_t minute_i);

		/** Set the internal time from a string.
		 * Parseable time formats are HH:MM, HH:M, H:MM and H:M.
		 * @returns the object.
		 * @throws std::invalid_argument if given string can't be parsed. */
		NaiveTime & operator=(const std::string & time_i);

		/** Return the internally stored time as a string.
		 * @returns Internal time in HH:MM format.
		 * @throws std::logic_error when internal hour and/or minute are not initialized. */
		operator std::string() const;

	}; // NaiveTime class

} // Fs2a namespace
