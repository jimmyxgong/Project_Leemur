#include "Terrain.h"

#include <chrono>
#include <random>


const double Terrain::randomness = 0.931322574615478515625e-9;
std::uniform_real_distribution<double> dis(-100, 100);

double Terrain::lerp(double a0, double a1, double w) const {
	//return ((1.0 - w) * a0) + (w * a1);
	//a0 *= 0.5;
	//a1 *= 0.5;
	double ww = 1.0 - w;
	double ww2 = ww * ww;
	double f0 = 3.0 * ww2 - 2.0 * ww2 * w;

	double w2 = w * w;
	double f1 = 3.0 * w2 - 2.0 * w2 * w;

	return a0 * f0 + a1 * f1;
}

double Terrain::noise(double x, double z) const {
	int n = x + (z * 57);
	n = (n << 13) ^ n;

	int step = (n * (n*n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(step) * randomness;
}

double Terrain::valueAt(double x, double z) const {
	int xFloor = (int) x;
	int zFloor = (int) z;
	double xremain = x - float(xFloor);
	double zremain = z - float(zFloor);

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

	//xremain = fade(xremain);
	//zremain = fade(zremain);

	double lerpc0 = lerp(corner00, corner10, xremain);
	double lerpc1 = lerp(corner01, corner11, xremain);

	return lerp(lerpc0, lerpc1, zremain);
}

double Terrain::valueAtS(double x, double z) const {
	return valueAt(x + seed, z + seed);
}

double Terrain::height(double x, double z) const {
	return height(x, z, frequency);
}

double Terrain::height(double x, double z, double freq) const {
	//x += 544;
	//z += 544;
	x += 512;
	z += 512;

	double value = 0;
	double amp = 1.0;

	for (int i = 0; i < octaves; i++) {
		double xFreq = x * freq;
		double zFreq = z * freq;

		value += valueAt(zFreq + seed, xFreq + seed) * amp;
		amp *= persistence;
		freq *= 2;
	}

	return (amplitude * value);
}

double Terrain::heightO(double x, double z) const {
	double value = 0;
	double amp = 1.0;
	double freq = frequency;
	for (int i = 0; i < octaves; i++) {
		double xFreq = x * freq;
		double zFreq = z * freq;

		value += perlinNoise(zFreq + seed, xFreq + seed) * amp;
		amp *= persistence;
		freq *= 1.8715;
	}
	return value / octaves;
}

double Terrain::perlinNoise(double x, double z) const {
	return perlinNoise(x, z, o1, o2, o3, o4, o5, o6);
}

double Terrain::perlinNoise(double x, double z, double o1, double o2, double o3, double o4, double o5, double o6) const {
	x *= frequency;
	z *= frequency;

	double e = o1 > 0 ? (o1 * valueAt(x, z)) : 1;
	double sum = o1 + o2 + o3 + o4 + o5 + o6;
	if (o2 > 0) e *= (o2 * valueAt(z * 2, x * 2));
	if (o3 > 0) e *= (o3 * valueAt(z * 4, x * 4));
	if (o4 > 0) e *= (o4 * valueAt(z * 8, x * 8));
	if (o5 > 0) e *= (o5 * valueAt(z * 16, x * 16));
	if (o6 > 0) e *= (o6 * valueAt(z * 32, x * 32));
	if (sum > 0) e = e / sum;

	return e;// pow(e, exp);
}

double Terrain::perlinNoise(double x, double y, double z, double parts, double div) const {
	using namespace std::chrono;

	double seed0 = seed;// system_clock::now().time_since_epoch().count();
	std::minstd_rand gen(seed0);
	double p0 = height(x + dis(gen) / parts, z + dis(gen) / parts) / div;
	double p1 = height(z + dis(gen) / parts, x + dis(gen) / parts) / div;
	double p2 = height(y / parts, x + dis(gen) / parts) / div;
	double p3 = height(z + dis(gen) / parts, y / parts) / div;

	return (int) round(p0 + p1 + p2 + p3 + y / 2);
}

int Terrain::gen(int rangeFrom, int rangeTo) const {
	const std::uniform_int_distribution<int> dis(rangeFrom, rangeTo);
	std::minstd_rand generator(std::chrono::system_clock::now().time_since_epoch().count());
	return dis(generator);
}

double Terrain::fade(double x) const {
	return (x*x*x*(x*(6 * x - 15) + 10));
}



Terrain & Terrain::setSeed(long long _seed) {
	seed = _seed;
	return *this;
}

Terrain & Terrain::setPersistence(double _persist) {
	persistence = _persist;
	return *this;
}

Terrain & Terrain::setFrequency(double _freq) {
	frequency = _freq;
	return *this;
}

Terrain & Terrain::setAmplitude(double _amp) {
	amplitude = _amp;
	return *this;
}

Terrain & Terrain::setOctaves(int _oct) {
	octaves = _oct;
	return *this;
}

Terrain & Terrain::setOct1(double o) {
	o1 = o;
	return *this;
}

Terrain & Terrain::setOct2(double o) {
	o2 = o;
	return *this;
}

Terrain & Terrain::setOct3(double o) {
	o3 = o;
	return *this;
}

Terrain & Terrain::setOct4(double o) {
	o4 = o;
	return *this;
}

Terrain & Terrain::setOct5(double o) {
	o5 = o;
	return *this;
}

Terrain & Terrain::setOct6(double o) {
	o6 = o;
	return *this;
}

Terrain & Terrain::setElevationExp(double e) {
	exp = e;
	return *this;
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




Terrain::Biome Terrain::biome(double e) {
	if (e == 3) return Biome::DESERT;
	if (e > 4) return Biome::MOUNTAINS;

	return Biome::PLAINS;
}