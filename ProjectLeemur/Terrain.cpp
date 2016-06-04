#include "Terrain.h"
#include "World.h"

#include <chrono>
#include <random>
#include <math.h>

const double Terrain::randomness = 0.931322574615478515625e-9;
std::uniform_real_distribution<double> dis(-100, 100);

// Hash lookup table as defined by Ken Perlin
const static int permutation[] = { 
	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,  
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

std::vector<int> Terrain::p;


double Terrain::lerp(double a0, double a1, double w) const {
	return ((1.0 - w) * a0) + (w * a1);
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

double Terrain::grad(int hash, double x, double y, double z) const {
	switch (hash & 0xFF) {
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0;
	}
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
	//const int size = RENDER_DISTANCE * CHUNK_SIZE;
	//x += -16;
	//z += -16;
	double value = 0;
	double amp = 1.0;

	double maxValue = 0;

	for (int i = 0; i < octaves; i++) {
		double xFreq = x * freq;
		double zFreq = z * freq;

		maxValue += amp;
		value += valueAt(xFreq + seed, zFreq + seed) * amp;
		amp *= persistence;
		freq *= 2;
	}

	return (amplitude * value / maxValue);
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
		freq *= 2;
	}
	return value / octaves;
}

double Terrain::perlinNoise(double x, double z) const {
	return perlinNoise(x, z, o1, o2, o3, o4, o5, o6);
}

double Terrain::snoise(double x, double z) const {
	return perlin(x, 128.0, z);
}

double Terrain::perlinNoise(double x, double z, double o1, double o2, double o3, double o4, double o5, double o6) const {
	const double epsilon = 0.0001;

	//x = x * frequency - 0.5;
	//z = z * frequency - 0.5;

	double sum = o1 + o2 + o3 + o4 + o5 + o6;
	double e = o1 > epsilon ? (o1 * snoise(x, z)) : 1;
	if (o2 > epsilon) e += (o2 * snoise(x * 2, z * 2));
	if (o3 > epsilon) e += (o3 * snoise(x * 4, z * 4));
	if (o4 > epsilon) e += (o4 * snoise(x * 8, z * 8));
	if (o5 > epsilon) e += (o5 * snoise(x * 16, z * 16));
	if (o6 > epsilon) e += (o6 * snoise(x * 32, z * 32));
	if (sum > epsilon) e /= sum;

	//e = abs(e);
	//if (e > 1) return 0;
	//e = abs(e);
	//if (e < 0) e = 0;

	//printf("e is: %.2f", e);
	if (e < 0) return 0;

	return e;// / 2 + 0.5;// pow(e, ex);
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

double Terrain::fade(double x) const {
	return (x*x*x*(x*(6 * x - 15) + 10));
}


int Terrain::inc(int num) const {
	num++;
	if (repeat > 0) num %= repeat;

	return num;
}

double Terrain::perlin(double x, double y, double z) const {
	if (repeat > 0) {		
		x = fmod(x, repeat);
		y = fmod(y, repeat);
		z = fmod(z, repeat);
	}
	// Bounding unit cube for normalization.
	int xi = (int) x & 255;
	int yi = (int) y & 255;
	int zi = (int) z & 255;

	//printf("x: %d, y: %d, z: %d", xi, yi, zi);

	int xr = x - (int)x;
	int yr = y - (int)y;
	int zr = z - (int)z;

	double u = fade(xr);
	double v = fade(yr);
	double w = fade(zr);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	//aaa = aba = aab = abb = baa = bba = bab = bbb = 0;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + inc(yi)] + zi];
	aab = p[p[p[xi] + yi] + inc(zi)];
	abb = p[p[p[xi] + inc(yi)] + inc(zi)];
	baa = p[p[p[inc(xi)] + yi] + zi];
	bba = p[p[p[inc(xi)] + inc(yi)] + zi];
	bab = p[p[p[inc(xi)] + yi] + inc(zi)];
	bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

	//double l1 = lerp(
	//	grad(aaa, xremain, 0, zremain),
	//	grad(baa, xremain - 1, 0, zremain),
	//	u
	//);

	//double l2 = lerp(
	//	grad(aab, xremain, 0, zremain - 1),
	//	grad(xremain - 1, 0, zremain - 1),
	//	u
	//);

	// The surrounding points in its unit cube.
	// This is all then lerped together as a sort of 
	// weighted average based on the faded (u,v,w)
	double x1, x2, y1, y2;
	x1 = lerp(
		grad(aaa, xr, yr, zr),          
		grad(baa, xr - 1, yr, zr),            
		u
	);										
	x2 = lerp(
		grad(aba, xr, yr - 1, zr),			
		grad(bba, xr - 1, yr - 1, zr),		
		u
	);

	x1 = lerp(
		grad(aab, xr, yr, zr - 1),
		grad(bab, xr - 1, yr, zr - 1),
		u
	);

	x2 = lerp(
		grad(abb, xr, yr - 1, zr - 1),
		grad(bbb, xr - 1, yr - 1, zr - 1),
		u
	);

	y1 = lerp(x1, x2, v);
	y2 = lerp(x1, x2, v);

	// normalize to 0-1 range
	return (lerp(y1, y2, w) + 1) / 2;
}

double Terrain::fbm(double x, double y, double z) const {
	double output = 0;
	double freq = frequency;
	double amp = 1;

	// used to normalize to range to 0:1
	double length = 0;

	for (int i = 0; i < octaves; i++) {
		length += amp;
		output += perlin(x * freq + seed, y * freq + seed, z * freq + seed) * amp;

		amp *= persistence;
		freq *= 2;
	}

	return output / length;
}


int Terrain::gen(int rangeFrom, int rangeTo) const {
	const std::uniform_int_distribution<int> dis(rangeFrom, rangeTo);
	std::minstd_rand generator(std::chrono::system_clock::now().time_since_epoch().count());
	return dis(generator);
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
	ex = e;
	return *this;
}



Terrain::Terrain() {
	if (p.size() < 512) {
		// Avoiding overflowing a buffer double the size.
		p.resize(512);
		for (int i = 0; i < 512; i++) {
			p[i] = permutation[i % 256];
		}
	}
}
Terrain::Terrain(long long seed) : seed(seed) {
	if (p.size() < 512) {
		p.resize(512);
		for (int i = 0; i < 512; i++) {
			p[i] = permutation[i % 256];
		}
	}
}

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
{
	if (p.size() < 512) {
		p.resize(512);
		for (int i = 0; i < 512; i++) {
			p[i] = permutation[i % 256];
		}
	}
}




Terrain::Biome Terrain::biome(double e) {
	if (e == 3) return Biome::DESERT;
	if (e > 4) return Biome::MOUNTAINS;

	return Biome::PLAINS;
}