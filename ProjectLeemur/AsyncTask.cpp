#include "AsyncTask.h"

template <typename T>
void AsyncTask::StartRoutine(std::generator<T> f) {
	if (!future.valid()) {
		future = aysnc(
			std::launch::async,
			[this]() {
			for (auto v : f()) {



			}
		});
		return;
	}

	auto status = future.wait_for(0ms);
	
}