#pragma once

#include "Commons.h"

#include <atomic>

#ifdef _WIN32
#include <future>
#include <pplawait.h>
#include <experimental/generator>
#endif

template <typename E>
class AsyncTask {
private:
	std::atomic<bool> canceled;
	std::future<E> future;

public:

	template <typename T>
	void StartRoutine(std::generator<T> f);

	void cancel();
};