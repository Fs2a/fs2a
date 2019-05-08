// vim:set ts=4 sw=4 noexpandtab:

#include "Semaphore.h"

namespace Fs2a {

	Semaphore::Semaphore(uint16_t count_i)
	: count_a(count_i), destructing_a(false)
	{ }

	Semaphore::~Semaphore()
	{
		destructing_a = true;
		convar_a.notify_all();
	}

	uint16_t Semaphore::count()
	{
		if (destructing_a) {
			throw std::logic_error("Semaphore is being destroyed while checking counter");
		}

		std::unique_lock<std::mutex> lck(mux_a);
		return count_a;
	}

	void Semaphore::operator--()
	{
		if (destructing_a) {
			throw std::logic_error("Semaphore is being destroyed while decreasing counter");
		}

		std::unique_lock<std::mutex> lck(mux_a);

		while (destructing_a == false && count_a == 0) {
			convar_a.wait(lck);
		}

		if (destructing_a) {
			throw std::logic_error("Semaphore is being destroyed while decreasing counter");
		}
		count_a--;
	}

	void Semaphore::operator++()
	{
		if (destructing_a) {
			throw std::logic_error("Semaphore is being destroyed while increasing counter");
		}

		std::unique_lock<std::mutex> lck(mux_a);
		count_a++;
		convar_a.notify_one();
	}

	bool Semaphore::try_dec()
	{
		if (destructing_a) {
			throw std::logic_error("Semaphore is being destroyed while decreasing counter");
		}

		std::unique_lock<std::mutex> lck(mux_a);
		if (count_a == 0) return false;
		count_a--;
		return true;
	}

} // Fs2a namespace
