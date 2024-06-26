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

#include <cstdint>
#include <condition_variable>
#include <list>
#include <future>
#include <mutex>
#include <stdexcept>
#include <thread>

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
			terminated /// No watches accepted, watcher thread terminated. Pending stuff is lost.
		};

		protected:
		/** Structure to hold STuff In Progress, i.e., stips. */
		struct stip {
			// Future where stip's result is stored
			std::future<ReturnType> result;

			// Auxiliary data to keep track of when running callback function
			std::shared_ptr<AuxData> aux;

			// Default constructor
			stip() = default;

			// Move constructor
			stip(stip && other) {
				result = std::move(other.result);
				aux = other.aux;
			}

			// Parameterized constructor
			stip(std::future<ReturnType> && result_i, std::shared_ptr<AuxData> aux_i) {
				result = std::move(result_i);
				aux = aux_i;
			}

			// Assignment operator
			stip & operator=(stip & other) {
				result = std::move(other.result);
				aux = other.aux;
				return *this;
			}

		};

		/** Internal mutex */
		mutable std::mutex mux_;

		/** Internal state */
		states state_;

		/** List with the actual tasks. */
		std::list<stip> stips_;

		/** Thread that actually monitors the tasks and calls the callbacks. */
		std::thread thr_;

		/** Condition to signal there is stuff to do. */
		std::condition_variable todo_;

		/** Watcher code to run in thread. */
		void watcher_() {
			while (true) {
				// Separate scope to make sure the unique lock releases every cycle
				{
					std::unique_lock<std::mutex> lck(mux_);
					switch (state_) {
						case states::pristine:
							throw std::logic_error(
								"Still in state pristine while watcher thread is running?"
							);
							break;

						case states::running:
							if (stips_.empty()) {
								todo_.wait(lck,[&]() { return stips_.size() > 0; });
							}
							break;

						case states::graceful:
							if (stips_.empty()) state_ = states::terminated;
							break;

						case states::terminated:
							break;
					}

					if (state_ == states::terminated) break;

					for (auto & item : stips_) {

					}
				} // unique_lock scope
			} // while (true)
		}

		public:
		/// Constructor
		ThreadedTasker() : state_(states::pristine) { }

		/** Default destructor terminates in progress stuff abruptly. */
		~ThreadedTasker() {
			terminate();
		}

		/** Graceful ending, wait for stuff in progress to finish. Doesn't accept new watches. */
		void graceful() {
			{
				std::unique_lock<std::mutex> lg(mux_);
				if (state_ != states::running) return; // No more need for graceful shutdown
				state_ = states::graceful;
			}
			if (thr_.joinable()) thr_.join();
		}

		/** Start the thread to watch tasks and run callbacks upon completion. */
		void start() {
			std::unique_lock<std::mutex> lg(mux_);
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

		/** Terminate immediately, not waiting for stuff that's in progress. Not recommended. */
		void terminate() {
			std::unique_lock<std::mutex> lck(mux_);
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
