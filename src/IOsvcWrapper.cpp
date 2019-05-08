// vim:set ts=4 sw=4 noexpandtab:

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
