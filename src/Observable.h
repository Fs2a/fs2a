/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2018
 * vim:set ts=4 sw=4 noexpandtab: */

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
			std::atomic<const T> value_a;

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
			const T get() { return value_a.load(); }

			/** Get the current list of observers. */
			const std::set<Observer<T> *> & observers() { return observers_a; }

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
