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

#include "IOsvcWrapper.h"

namespace Fs2a {

	IOsvcWrapper::IOsvcWrapper()
	: Fs2a::Singleton<IOsvcWrapper>::Singleton()
	{
	}

	IOsvcWrapper::~IOsvcWrapper()
	{
		this->stop();
	}

	void IOsvcWrapper::runHere()
	{
		this->stop();
		iosvc_a.run();
	}

	void IOsvcWrapper::runThread()
	{
		this->stop();
		thread_a.reset(new std::thread([this]() { iosvc_a.run(); }));
	}

	void IOsvcWrapper::stop()
	{
		bool oldSWI = stopWhenIdle();

		if (!oldSWI) stopWhenIdle(true);
		if (!iosvc_a.stopped()) iosvc_a.stop();
		if (thread_a) {
			thread_a->join();
			thread_a.reset();
		}

		iosvc_a.reset();
		stopWhenIdle(oldSWI);
	}

	void IOsvcWrapper::stopWhenIdle(const bool stopWhenIdle_i)
	{
		if (stopWhenIdle_i) {
			if (work_a) work_a.reset();
		} else {
			if (!work_a) work_a.reset(
				new boost::asio::io_service::work(iosvc_a)
			);
		}
	}

} // Fs2a namespace
