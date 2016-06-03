#pragma once

#include "Commons.h"

struct Terrain {
public:
	enum Biome {
		PLAINS,
		MOUNTAINS,
		DESERT
	};

public:
	double persistence;
	double frequency;
	double amplitude;
	int octaves;
	long long seed;
	double lacunarity;

	double o1 = 0;
	double o2 = 0;
	double o3 = 0;
	double o4 = 0;
	double o5 = 0;
	double o6 = 0;
	double ex = 1;

	const static double randomness;

	double lerp(double x, double z, double weight) const;
	double noise(double x, double z) const;
	double valueAt(double x, double z) const;
	double valueAtS(double x, double z) const;
	double height(double x, double z) const;
	double height(double x, double z, double freq) const;
	double heightO(double x, double z) const;
	double perlinNoise(double x, double y, double z, double parts, double div) const;
	double fade(double x) const;

	double perlinNoise(double x, double z) const;
	double snoise(double x, double z) const;
	double perlinNoise(double x, double z, double o1, double o2, double o3, double o4, double o5, double o6) const;

	int gen(int rangeFrom, int rangeTo) const;

	Terrain & setSeed(long long seed);
	Terrain & setPersistence(double persist);
	Terrain & setFrequency(double freq);
	Terrain & setAmplitude(double amp);
	Terrain & setOctaves(int octave);

	Terrain & setOct1(double o);
	Terrain & setOct2(double o);
	Terrain & setOct3(double o);
	Terrain & setOct4(double o);
	Terrain & setOct5(double o);
	Terrain & setOct6(double o);

	Terrain & setElevationExp(double e);

	Terrain(long long seed);
	Terrain(double persist,
			double freq,
			double amp,
			int octaves,
			long long seed);




	Biome biome(double e);
};