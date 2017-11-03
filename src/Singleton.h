/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) 2017 */

#pragma once

#include <memory>
#include <mutex>

namespace Fs2a {

	template <class T>
	class Singleton {
		protected:
		/// Internal pointer to instance
		static std::unique_ptr<T> instance_a;

		/// Mutex to prevent race conditions upon construction
		static std::mutex mux_a;

		/// Default constructor
		inline Singleton<T>() {};

		/// Copy constructor
		Singleton<T>(const Singleton<T> & obj_i) = delete;

		/// Assignment constructor
		Singleton<T> & operator=(const Singleton<T> & obj_i) = delete;

		/// Destructor
		~Singleton<T>();

		public:
		/** Get the Singleton instance pointer.
		 * @returns a pointer to the singleton instance. */
		static inline T *instance() {
			std::lock_guard<std::mutex> lck(mux_a);
			if (!instance_a) instance_a.reset(new T());
			return instance_a.get();
		}

		/** Explicitly close the singleton */
		static inline void close() {
			std::lock_guard<std::mutex> lck(mux_a);
			instance_a.reset(nullptr);
		}

		static inline bool is_constructed() {
			std::lock_guard<std::mutex> lck(mux_a);
			return (bool) instance_a;
		}

	};

	template <class T> std::unique_ptr<T> Singleton<T>::instance_a;
	template <class T> std::mutex Singleton<T>::mux_a;

} // Fs2a namespace
