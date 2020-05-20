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

#include <mutex>
#include "Observable.h"

namespace Fs2a {

	template <class T>
	class Observable;

	template <class T>
	class Observer
	{
		private:
			// Mutex to prevent race conditions
			std::mutex observerMux_a;

			// Observable we are observing
			Observable<T> *observable_a;

		public:
			/** Observer constructor. */
			Observer() : observable_a(nullptr) {}

			/** Observer destructor. Unsubscribes from Observable if
			 * previously subscribed. */
			~Observer() {
				observe(nullptr); // Unsubscribe if previously subscribed
			}

			/** Get the current observable object being observed.
			 * @returns A pointer to the observed object,
			 * NULL if no object is being observed. */
			Observable<T> * observable() const { return observable_a; }

			/** Start observing an Observable object. Calling this method
			 * ensures ignoring the object again when this class is destroyed, which
			 * will prevent segmentation faults.
			 * @param observableI Observable to start observing. If
			 * observableI is NULL, this method starts ignoring the object if previously
			 * observing. When another object was previously being
			 * observed, that object will start being ignored.
			 * @returns True if the observable object was changed (both in
			 * this object and in the observable object), false if not. */
			bool observe(Observable<T> *observableI = nullptr) {
				std::lock_guard<std::mutex> lg(observerMux_a);
				if (observableI == nullptr) {
					if (observable_a == nullptr) return false;
					bool rv = observable_a->removeObserver(this);
					observable_a = nullptr;
					return rv;
				} else {
					if (observableI == observable_a) return false;
					if (observable_a != nullptr) {
						return observable_a->removeObserver(this);
					}
					observable_a = observableI;
					return observable_a->addObserver(this);
				}
			}

			/** Called when the value in the Observable class is updated.
			 * Please do only little processing here, since this method is
			 * called from the Observable class thread that updated the
			 * value.
			 * @param valueI The new value that was set. */
			virtual void update(const T & valueI) = 0;
	};

} // Fs2a namespace
