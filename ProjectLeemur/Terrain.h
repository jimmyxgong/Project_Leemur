#pragma once

#include "Commons.h"

struct Terrain {
public:
	double persistence;
	double frequency;
	double amplitude;
	int octaves;
	long long seed;

	const static double randomness;

	double lerp(double x, double z, double weight) const;
	double noise(double x, double z) const;
	double valueAt(double x, double z) const;
	double height(double x, double z) const;
	double perlinNoise(double x, double y, double z, double parts, double div) const;


	Terrain(long long seed);
	Terrain(double persist,
			double freq,
			double amp,
			int octaves,
			long long seed);
};