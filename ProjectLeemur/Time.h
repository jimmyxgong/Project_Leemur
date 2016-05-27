#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include <chrono>

using namespace std;
using namespace chrono;

class Time {
private:
	time_point<steady_clock> lastTime;
	double delta = 0.0f;
	double limit;
	void init();

	bool started = false;

public:
	Time(float frameLimit);

	void start();
	bool ready();

	double getDelta() const;
};

#endif // !GAME_TIMER_H