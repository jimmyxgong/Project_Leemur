#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <pplawait.h>
#include <experimental/generator>

using namespace std::experimental;
using routine = generator<std::chrono::nanoseconds>;

class AsyncTask {
private:
	std::atomic<bool> online;
	std::atomic<bool> canceled;
	std::future<void> future;
	std::function<routine()> savedTask;
	
	void startTask(routine & task);

public:

	void StartCoroutine(routine &);

	void restart();
	void cancel();
	bool isValid() const;
	bool isRunning() const;

	explicit AsyncTask();
};