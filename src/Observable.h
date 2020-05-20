/* Copyright (c) 2020 Simon de Hartog <simon@dehartog.name>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include <atomic>
#include <mutex>
#include <set>
#include "Observer.h"

namespace Fs2a {

	// Forward declaration to prevent inclusion loops
	template <class T>
	class Observer;

	template <class T>
	class Observable
	{
		// Allow Observers to add and remove themselves.
		friend class Observer<T>;

		private:
			// Mutex to prevent race conditions
			std::mutex observableMux_a;

			// Set of observers
			std::set<Observer<T> *> observers_a;

			// Keep track of the actual value
			std::atomic<T> value_a;

			/** Add an observer to this observable to receive future value updates.
			 * @param observerI Observer that will be observing us.
			 * @returns True if @p observerI is newly observing us, false if
			 * it was already there. */
			bool addObserver(Observer<T> *observerI) {
				std::lock_guard<std::mutex> lg(observableMux_a);
				auto p = observers_a.insert(observerI);
				return p.second;
			}

			/** Remove an observer to prevent future updates.
			 * @param observerI Observer that is to be removed.
			 * @returns True if @p observerI was removed, false if
			 * it was not present in the first place. */
			bool removeObserver(Observer<T> *observerI) {
				std::lock_guard<std::mutex> lg(observableMux_a);
				return observers_a.erase(observerI) > 0;
			}

		public:

			// Constructor to initialize internal value
			Observable(const T & valueI)
				: value_a(valueI) {}

			// Destructor
			~Observable() {}

			// Retrieve the value
			const T get() const { return value_a.load(); }

			/** Get the current list of observers. */
			const std::set<Observer<T> *> & observers() const { return observers_a; }

			/** Set the value and broadcast it if it actually changed.
			 * @param valueI The new value to set. */
			void set(const T & valueI) {
				if (valueI == value_a) return;
				std::lock_guard<std::mutex> lg(observableMux_a);
				value_a.store(valueI);
				for (auto const & i: observers_a) {
					i->update(value_a.load());
				}
			}

	};

} // Fs2a namespace
