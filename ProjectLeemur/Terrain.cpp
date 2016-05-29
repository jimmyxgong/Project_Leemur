#include "Terrain.h"


const double Terrain::randomness = 0.931322574615478515625e-9;

double Terrain::lerp(double a0, double a1, double w) const {
	return ((1.0 - w) * a0) + (w * a1);
}

double Terrain::noise(double x, double z) const {
	int n = x + (((int)z) << 6);
	n = (n << 13) ^ n;

	int step = (n * (n*n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(step) * randomness;
}

double Terrain::valueAt(double x, double z) const {
	int xFloor = (int) x;
	int zFloor = (int) z;
	double xremain = x - xFloor;
	double zremain = z - zFloor;

	double n00 = noise(xFloor - 1, zFloor - 1);
	double n01 = noise(xFloor + 1, zFloor - 1);
	double n02 = noise(xFloor - 1, zFloor + 1);
	double n03 = noise(xFloor + 1, zFloor + 1);
	double n04 = noise(xFloor - 1, zFloor);
	double n05 = noise(xFloor + 1, zFloor);
	double n06 = noise(xFloor, zFloor - 1);
	double n07 = noise(xFloor, zFloor + 1);
	double n08 = noise(xFloor, zFloor);

	double n10 = noise(xFloor + 2, zFloor - 1);
	double n11 = noise(xFloor + 2, zFloor + 1);
	double n12 = noise(xFloor + 2, zFloor);

	double n20 = noise(xFloor - 1, zFloor + 2);
	double n21 = noise(xFloor + 1, zFloor + 2);
	double n22 = noise(xFloor, zFloor + 2);
	
	double n30 = noise(xFloor + 2, zFloor + 2);

	constexpr static double l = 0.0625, r = 0.125, p = 0.25;
	double corner00 = l*(n00 + n01 + n02 + n03) + r*(n04 + n05 + n06 + n07) + p*(n08);
	double corner10 = l*(n06 + n10 + n07 + n11) + r*(n08 + n12 + n01 + n03) + p*(n05);
	double corner01 = l*(n04 + n05 + n20 + n21) + r*(n02 + n03 + n08 + n22) + p*(n07);
	double corner11 = l*(n08 + n12 + n22 + n30) + r*(n07 + n11 + n05 + n21) + p*(n03);

	double lerpc0 = lerp(corner00, corner10, xremain);
	double lerpc1 = lerp(corner01, corner11, xremain);

	return lerp(lerpc0, lerpc1, zremain);
}

double Terrain::height(double x, double z) const {
	double value = 0;
	double amp = 1;
	double freq = frequency;

	for (int i = 0; i < octaves; i++) {
		double xFreq = x * freq;
		double zFreq = z * freq;

		value += valueAt(zFreq + seed, xFreq + seed) * amp;
		amp *= persistence;
		freq *= 2;
	}
	return amplitude * value;
}




Terrain::Terrain(long long seed) : seed(seed) {}

Terrain::Terrain(
	double persist, 
	double freq, 
	double amp, 
	int oct, 
	long long seed
):
	persistence(persist), 
	frequency(freq), 
	amplitude(amp), 
	octaves(oct), 
	seed(seed) 
{ }