#include "Time.h"

#define NANO_S 1000000000.0

Time::Time(float frameLimit) {
	limit = NANO_S / frameLimit;
	init();
}

void Time::init() {

}

void Time::start() {
	if (!started) {
		lastTime = high_resolution_clock::now();
		started = true;
	}
}

bool Time::ready() {
	auto now = high_resolution_clock::now();
	nanoseconds duration = duration_cast<nanoseconds>(now - lastTime);
	delta += double(duration.count()) / (limit);
	lastTime = now;
	if (delta >= 1) {
		delta = 0;
		return true;
	}
	return false;
}

double Time::getDelta() const {
	return delta;
}

long Time::getTime() {
	return steady_clock::now().time_since_epoch().count();
}