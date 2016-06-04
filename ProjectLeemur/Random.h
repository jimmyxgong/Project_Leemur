#pragma once
#include "Commons.h"

#include <random>

class Random {
private:
	static long long seed;
	static std::minstd_rand gen;

public:

	static int Range(int from, int to);
	static double Range(double from, double to);

	static void setSeed(long long seed);
	static void setSeedToCurrentTime();
};