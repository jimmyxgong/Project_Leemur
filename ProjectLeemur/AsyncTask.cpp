#include "AsyncTask.h"

void t(routine & v) {
	for (auto t : v) {

	}
}

void AsyncTask::startTask(routine & task) {
	using namespace std;
	using namespace std::chrono;

	future = async(std::launch::async, [&task, this]() {
		online = true;
		for (auto time : task) {
			if (canceled) break;
			this_thread::sleep_for(time);
		}
		canceled = false;
		online = false;
	});
}
void AsyncTask::StartCoroutine(routine & gen) {
	using namespace std;
	using namespace std::chrono;

	if (!future.valid()) {
		startTask(gen);
		return;
	}

	auto status = future.wait_for(0ms);
	if (status == std::future_status::ready) {
		startTask(gen);
		return;
	}

	cancel();
	startTask(gen);
}

void AsyncTask::cancel() {
	canceled = true;
}

bool AsyncTask::isValid() const {
	return canceled;
}

bool AsyncTask::isRunning() const {
	return online;
}

AsyncTask::AsyncTask() 
	: canceled(false),
	online(false)
{

}