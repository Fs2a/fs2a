/* BSD 3-Clause License
 *
 * Copyright (c) 2024, Fs2a, Bren de Hartog <bren@fs2a.pro>
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

#include <atomic>
#include <chrono>
#include <cstdint>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <Logger.h>

using namespace std::chrono_literals;

namespace Fs2a {

	template <typename ReturnType = void, typename AuxData = std::nullptr_t>
	class ThreadedTasker
	{
		public:
		/** Enum for internal state. */
		enum class states : uint8_t {
			pristine, /// Pristine startup state. Watches are accepted, but not monitored.
			running, /// Running. Watches are accepted and callbacks are called upon completion.
			graceful, /// Graceful shutdown. No new watches accepted, but still call callbacks.
			stopped /// No watches accepted, watcher thread stopped. Pending stuff is lost.
		};

		protected:
		/** Structure to hold STuff In Progress, i.e., stips. */
		struct stip {
			// Auxiliary data to keep track of when running callback function
			std::shared_ptr<AuxData> aux;

			// Callback function
			std::function<void(std::future<ReturnType>, std::shared_ptr<AuxData>)> callback;

			// Future where stip's result is stored
			std::future<ReturnType> result;

			// Default constructor
			stip() = default;

			// Move constructor
			stip(stip && other) {
				aux = other.aux;
				callback = other.callback;
				result = std::move(other.result);
			}

			// Parameterized constructor
			stip(
				std::future<ReturnType> && result_i,
				std::function<void(std::future<ReturnType>, std::shared_ptr<AuxData>)> callback_i,
				std::shared_ptr<AuxData> aux_i
			) {
				aux = aux_i;
				callback = callback_i;
				result = std::move(result_i);
			}

			// Assignment operator
			stip & operator=(stip & other) {
				aux = other.aux;
				callback = other.callback;
				result = std::move(other.result);
				return *this;
			}

		};

		/** Internal mutex */
		mutable std::mutex mux_;

		/** Internal state */
		std::atomic<states> state_;

		/** List with the actual tasks. */
		std::list<stip> stips_;

		/** Condition for inter-thread synchronization. */
		std::condition_variable sync_;

		/** Thread that actually monitors the tasks and calls the callbacks. */
		std::thread thr_;

		/** Watcher code to run in thread. */
		void watcher_() {
			while (state_.load() != states::terminated) {
				// Separate scope to make sure the unique lock releases every cycle
				{
					std::unique_lock<std::mutex> lck(mux_);
					switch (state_.load()) {
						case states::pristine:
							throw std::logic_error(
								"Still in state pristine while watcher thread is running?"
							);
							break;

						case states::running:
							if (stips_.empty()) {
								sync_.wait(lck,[&]() {
									return state_.load() != states::running || !stips_.empty();
								});
							}
							break;

						case states::graceful:
							if (stips_.empty()) state_.store(states::terminated);
							break;

						case states::terminated:
							break;
					}

					// Iterator to delete once moved on in the loop
					auto todel = stips_.end();

					for (auto i = stips_.begin(); i != stips_.end(); i++) {
						// Do this after continuing the loop, otherwise we would
						// invalidate the loop iterator.
						if (todel != stips_.end()) {
							stips_.erase(todel);
							todel = stips_.end():
						}
						if (state_.load() == states::terminated) break;
						if (!i->result.valid()) {
							todel = i;
							continue;
						}
						switch (i->result.wait_for(0s)) {
							case std::future_status::deferred:
								// Deferred tasks should never have entered the list
								todel = i;
								continue;

							case std::future_status::timeout:
								// Simply not done yet
								break;

							case std::future_status::ready:
								// Task is finally done, call callback with lock released
								lck.unlock();
								try {
									// Don't let callback mess up our thread
									i->callback(i->result, i->aux);
								} catch (...) {
									// Ignore callback exceptions
								}

								// Lock released and callback ran, yield a bit
								std::this_thread::yield();

								lck.lock();
								todel = i;
								continue;
						} // switch result.wait_for(0s)
					} // for
				} // unique_lock scope
				std::this_thread::yield();
			} // while
		} // watcher_()

		public:
		/// Constructor
		ThreadedTasker() : state_(states::pristine) { }

		/** Default destructor calls stop() for cleanup */
		~ThreadedTasker() {
			stop();
		}

		/** Graceful ending, wait for stuff in progress to finish. Doesn't accept new watches. */
		void graceful() {
			{
				std::unique_lock<std::mutex> lck(mux_);
				switch (state_) {
					case states::pristine:
						state_ = states::terminated;
						return;

					case states::graceful:
						/** This indicates another thread decided to wait on our graceful
						 * termination, so do that here. */
						sync_.wait(lck, [&]() { state_.load() != states::graceful; });
						return;

					case states::terminated:
						// Nothing left to do
						return;

					case states::running:
						state_ = states::graceful;
						break;
				}
			}
			// Only reachable if state was changed from running to graceful
			if (thr_.joinable()) thr_.join();
		}

		/** Reset object to pristine state, if possible. */
		void reset() {
			std::unique_lock<std::mutex> lck(mux_);
			if (thr_.joinable()) throw std::runtime_error(
				"Unable to reset ThreadedTasker when thread is running");
			switch (state_) {
				case states::terminated:
					// Reset back to pristine
					state_ = states::pristine;
					// Fall-through for cleanup

				case states::pristine:
					// Already in pristine state, clear list to be sure
					stips_.clear();
					return;

				default:
					throw std::runtime_error(
						"Unable to reset ThreadedTasker when running");
			}
			return;
		}

		/** Return the number of things currently in progress. */
		size_t size() {
			std::unique_lock<std::mutex> lck(mux_);
			return stips_.size();
		}

		/** Start the thread to watch tasks and run callbacks upon completion. */
		void start() {
			std::unique_lock<std::mutex> lck(mux_);
			if (state_ != states::pristine) {
				throw std::logic_error(
					"Unable to start ThreadedTasker thread when not in pristine state"
				);
			}
			if (thr_.joinable()) {
				throw std::runtime_error(
					"ThreadedTasker in pristine state, but thread is joinable?"
				)
			}
			state_ = state::running;
			thr_ = std::thread(&ThreadedTasker::watcher_, this);
		}

		/** Get the current state the object is in. */
		states state() const { return state_; }

		/** Stop immediately. No callbacks are called. The watched tasks may still be waited for
		 * though. C++ does not allow us to simply ignore and forget an async task. */
		void stop() {
			std::unique_lock<std::mutex> lck(mux_);
			switch (state_) {
				case states::graceful:
					// There is a graceful shutdown in progress, can't stop that
					sync_.wait(lck, [&]() { return !(thr_.joinable()); });
					return;

			state_ = states::terminated;
			if (thr_.joinable()) thr_.join();
		}

		/** Watch a new future for completion.
		 * @param future_i Future to watch
		 * @param callback_i Callback function to call when future is done. */
		void watch(
			std::future<ReturnType> future_i,
			std::function<void(std::future<ReturnType>, std::shared_ptr<AuxData>)> callback_i
		);
	};

} // Fs2a namespace
