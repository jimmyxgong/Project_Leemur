#include "Random.h"

#include <chrono>
#include <typeinfo>

long long Random::seed;
std::minstd_rand Random::gen;

int Random::Range(int from, int to) {
	std::uniform_int_distribution<int> dist(from, to);
	return dist(gen);
}

double Random::Range(double from, double to) {
	std::uniform_real_distribution<double> dist(from, to);
	return dist(gen);
}


//template <typename T>
//T Random::Range(T from, T to) {
//	if (typeid(T) == typeid(int)) {
//		std::uniform_int_distribution<int> dist(from, to);
//		return dist(gen);
//	}
//	else if (typeid(T) == typeid(double) || typeid(T) == typeid(float)) {
//		std::uniform_real_distribution<T> dist(from, to);
//		return dist(gen);
//	}
//
//	return 0;
//}

void Random::setSeed(long long _seed) {
	seed = _seed;
	gen = std::minstd_rand(_seed);
}

void Random::setSeedToCurrentTime() {
	using namespace std::chrono;
	setSeed(system_clock::now().time_since_epoch().count());
}