/* vim:set ts=4 sw=4 noet tw=90:
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2020-2022, Fs2a, Simon de Hartog <simon@fs2a.pro>
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#pragma once

#include <mutex>
#include <unordered_set>
#include "Observer.h"

namespace Fs2a {
	/** Keep track of observers that are observing us and notify them when
	 * the observed value changes. Everything is administered through weak
	 * pointers to prevent destruction deadlocks and shared pointer loops.
	 */
	template <class T>
	class Observable
	{
		private:
			// Mutex to prevent race conditions
			std::mutex mux_;

			// Set of observers
			std::unordered_set<std::weak_ptr<Observer<T>>> observers_;

			// Keep track of the actual value
			T value_;

			/** Safety method that gets the weak shared pointer from the
			 * given observer to prevent segfaults when propagating changes
			 * to the observed value.
			 * @returns False when not actually registered, true when it
			 * is. */
			bool addObserver(Observer<T> * observer_i) {
				if (observer_i == nullptr) return false;
				return addObserver(observer_i->weak_from_this());
			}

			/** Add an observer to this observable to receive future value updates.
			 * @param observer_i Weak pointer to Observer that will be observing us.
			 * @returns True if @p observer_i is newly observing us, false if
			 * it was already there or if the given @p observer_i weak
			 * pointer has expired. */
			bool addObserver(std::weak_ptr<Observer<T>> observer_i) {
				if (observer_i.expired()) return false;
				std::lock_guard<std::mutex> lg(mux_a);
				auto p = observers_.insert(observer_i);
				return p.second;
			}

			/** Remove an observer to prevent future updates.
			 * @param observerI Observer that is to be removed.
			 * @returns True if @p observerI was removed, false if
			 * it was not present in the first place. */
			bool removeObserver(Observer<T> *observerI) {
				std::lock_guard<std::mutex> lg(mux_a);
				return observers_a.erase(observerI) > 0;
			}

		public:

			// Constructor to initialize internal value
			Observable(const T & value_i)
				: value_(value_i) {}

			// Destructor
			~Observable() {}

			// Retrieve the value
			T get() const { return value_; }

			/** Get the current list of observers. */
			const std::unordered_set<std::weak_ptr<Observer<T>>> & observers() const { return observers_; }

			/** Set the value and broadcast it if it actually changed.
			 * @param value_i The new value to set.
			 * @returns True if value was changed and broadcasted to
			 * observers, false if value is unchanged. */
			bool set(const T & value_i) {
				std::unique_lock<std::mutex> lck(mux_);
				if (value_i == value_) return false;
				value_ = value_i;

				/** Make a deep copy of all the Observers To Be Notified so
				 * we can unlock the mutex to prevent deadlocks. */
				std::unordered_set<std::weak_ptr<Observer<T>>> otbn = observers_;
				lck.unlock();
				bool foundExpired = false;
				for (std::weak_ptr<Observer<T>> i: otbn) {
					// Shared Pointer to Observer
					auto spo = i.lock();
					if (!spo) {
						foundExpired = true;
						continue;
					}
					spo->observableChanged(value_);
				}

				if (foundExpired) {
					lck.lock();
					for (auto it = observers_.begin(); it != observers_.end(); ) {
						if (it->expired()) it = observers_.erase(it);
						else it++;
					}
				}

				return true;
			}

	};

} // Fs2a namespace
