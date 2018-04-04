/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

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
