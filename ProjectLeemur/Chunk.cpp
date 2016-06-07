#include "Chunk.h"
#include "Terrain.h"
#include "SolidCell.h"
#include "Resources.h"
#include "Light.h"
#include "Window.h"
#include "Resources.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Environment.h"
#include "World.h"
#include "Random.h"
#include "Time.h"
#include "MeshData.h"
#include "Building.h"
#include "Turtle.h"
#include "LSystem.h"

#ifdef _WIN32
#include <future>
#include <pplawait.h>
#endif

#define PARTS 1.2 // 3.2
#define DIV 4.3	// 0.7
#define CUTOFF 3.3 // 5.3

#define TREE1 "tree1.txt"
#define TREE2 "tree2.txt"
#define TREE3 "tree3.txt"

// persist 7.2
//Terrain terrain = {
//	0.25, 1, 1, 6, 100	//6.7, 0.6, 1.0, 8, 1 
//}; //0.05, 1, 1, 6, 100
//// parts 10.2 div 20.7 cutoff 9.8
//
//Keyboard::Layout bindings;

//template <typename T>
//using await = __await T;

UniquePointer<Chunk> Chunk::EMPTY = unique<Chunk>(true);



void Chunk::onCreate() {
	resizeStructure();
}

void Chunk::onStart() {
	// Happens after all chunks are created.

	buildMeshData();
	mesh.recalculateNormals();
	mesh.init();
}

void Chunk::onRender() {
	loadToShader();
	mesh.render();

	for (WeakPointer<Building> build : buildings) {
		build.lock()->onRender();
	}

	for (WeakPointer<Turtle> plant : plants) {
		plant.lock()->onRender();
	}
}

void Chunk::onUpdate() {
	resumeWaves();

	if (changed) {
		printf("\nUpdating\n");
		renderMesh();
		changed = false;
	}
}

void Chunk::onDestroy() {
	canceled = true;
	BaseEntity::onDestroy();
	mesh.destroy();
}



void Chunk::resizeStructure() {
	heightMap.resize(CHUNK_SIZE);
	dheightMap.resize(CHUNK_SIZE);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		heightMap[i].resize(CHUNK_SIZE);
		dheightMap[i].resize(CHUNK_SIZE);
	}
}

void Chunk::loadToShader() {
	const World::Biomes biome = world->biomeOptions;
	static Skybox & skybox = (Skybox &)Resources::getEntity(SKYBOX);
	Resources::getShader(TERRAIN_LIGHT).use();

	Light::Directional.loadToShaderi();
	Material::Grass.loadToShader();
	Material::Snow.loadToShader();
	Material::Sand.loadToShader();
	Material::Water.loadToShader();
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());

	Shader::loadFloat("waterHeight", biome.waterMax)
		.loadFloat("sandHeight", biome.sand)
		.loadFloat("snowHeight", biome.snow);

	Matrix4f model = transform.getLocalToWorldMatrix();
	Matrix4f MVP = Window::getFocusedWindow().getPerspective()
		* Window::getFocusedWindow().getView()
		* model;

	Shader::loadMatrix("MVP", MVP);
	Shader::loadMatrix("model", model);
	Shader::loadMatrix("NormalMatrix", Matrix3f(transpose(inverse(model))));

	// TODO: there might be some side-effects of using CAMERA_POSITION
	// instead of using the actual Window active camera. Too lazy to
	// write another method to get the active camera.
	Camera & cam = Window::getFocusedWindow().getActiveCamera();
	Shader::loadVector("CameraPosition", cam.transform.getLocalPosition());
}












void Chunk::generateChunk(Terrain & terrain) {
	if (!world->options.isWaterDecreasing) {
		if (buildings.size() > 0) buildings.clear();
		if (plants.size() > 0) plants.clear();
	}
	

	Random::setSeedToCurrentTime();
	hasWater = false;
	//clear();

	const Vector3f pos = transform.getPosition();
	const double roundedX = round(pos.x);
	const double roundedZ = round(pos.z);

	const World::Biomes & biome = world->biomeOptions;

	const float freq = PLANT_BUILDING_FREQ_RENDER / biome.frequency;
	const static int NUM_OF_GENERATIONS = 1;
	int count = 0;

	Terrain placement = terrain;
	placement.setSeed(terrain.seed * 2 + 1);

	for (int i = 0; i < CHUNK_SIZE; i++) {
		double x = i + roundedX;
		for (int k = 0; k < CHUNK_SIZE; k++) {
			double z = k + roundedZ;

			double h = 0;
			//if (!biome.restructureParts) {
				double g = terrain.height(x, z);
				h = ((int)round(g * HEIGHT_CONSTANT)) * HEIGHT_UNIT;
				if (h < world->biomeOptions.waterMax + 0.02) {
					h = world->biomeOptions.waterMax - 0.01;
					h -= Random::Range(0.0, 0.3);
					hasWater = true;
				}
				dheightMap[i][k] = h;
			//}
			//else h = dheightMap[i][k];

			if (world->options.isWaterDecreasing) continue;

			double m = placement.valueAt(x, z);
			if (biome.frequency <= PLANT_BUILDING_FREQ_RENDER
				&& count < NUM_OF_GENERATIONS
				&& m < biome.waterMin
				&& Random::Range(0, 10000) < 100)
			{
				if (world->options.generateBuildings) {
					if (h > biome.buildingMin && h < biome.buildingMax) {
						Vector3f size = Vector3f(0.35f, 1.5f, 0.35f);
						size *= freq;

						SharedPointer<Building> build =
							share<Building>(Vector3f(x, h - 0.01, z), size, 
								(int)round(Random::Range(0, 1000000) * m));
						buildings.push_back(build);
						count++;
					}
				}
				if (count < NUM_OF_GENERATIONS
					&& world->options.generatePlants
					&& h > biome.plantMin && h < biome.plantMax) {
					Vector3f position = Vector3f(x, h, z);
					LSystem plant = LSystem(
						Random::Range(0, 10) >= 5
						? Random::Range(0, 10) >= 5
						? TREE1
						: TREE3
						: TREE2);

					plant.setSeed((long long)round(Random::Range(0, 100000) * m));
					plant.turtle->branches->setLocalPosition(position);
					plant.turtle->leaves->setLocalPosition(position);
					plant.turtle->branches->scaleLocal(0.4f * freq);
					plant.turtle->leaves->scaleLocal(0.4f * freq);

					plant.drawRules();

					plants.push_back(plant.turtle);
					count++;
				}
			}
		}
	}

	Random::setSeedToCurrentTime();
	if (world->options.isWaterDecreasing) return;

	for (WeakPointer<Building> build : buildings) {
		build.lock()->onCreate();
	}
	for (WeakPointer<Building> build : buildings) {
		build.lock()->onStart();
		build.lock()->onUpdate();
	}
	for (WeakPointer<Turtle> plant : plants) {
		plant.lock()->onCreate();
	}
	for (WeakPointer<Turtle> plant : plants) {
		plant.lock()->onStart();
		plant.lock()->onUpdate();
	}
}
















// When there are two chunks: we need to interwove their vertices together
// while also maintaining their out of bounds checking.
Vector3f Chunk::outOfBoundsVertex(double x, double z, int fi, int fk, int i, int k) {
	const double d = HEIGHT_UNIT;

	int oi = 0;
	int ok = 0;
	int ii = 0;
	int kk = 0;
	bool outOfBounds = false;

	// out of bounds in x coordinate
	if (isOutOfBounds(fi + i, 0, fk)) {
		outOfBounds = true;
		kk = fk + k;
		oi = 1;
	}

	// out of bounds in z coordinate
	if (isOutOfBounds(fi, 0, fk + k)) {
		ii = fi + i;
		ok = 1;
		if (outOfBounds) {
			ii = 0;
			kk = 0;
		}

		outOfBounds = true;
	}

	double y = 0;
	if (outOfBounds) {
		// Get neighboring chunk
		
		std::string key = world->toKey(mapPosition.x + oi, mapPosition.z + ok);

		//Array<Array<double> const & map = world->getChunk(key).getHeightMap();
		//mesh.addVertex(x + i, map[ii][kk] * d, z + k);
		//return;
		y = world->getChunk(key).getHeightMap()[ii][kk];
	}
	else y = getHeightMap()[fi + i][fk + k];

	//mesh.addVertex(x + i, y, z + k);
	return {x + i, y, z + k};
}



void Chunk::buildMeshData() {
	Random::setSeedToCurrentTime();

	const Vector3f pos = transform.getPosition();
	const double d = HEIGHT_UNIT;

	// Generate vertices: optimized way, but too much of a hassle for now :/
	//for (int i = 0; i < CHUNK_SIZE; i++) {
	//	double displace = i % 2 == 0 ? 0 : 0.5;
	//	for (int k = 0; k < CHUNK_SIZE; k++) {
	//		double y = getHeightMap()[i][k];
	//		double x = i + pos.x;
	//		double z = k + pos.z;
	//		mesh.addVertex(x, y, z + displace);
	//	}
	//}

	bool chunk10 = !world->containsKey(mapPosition.x + 1, mapPosition.z);
	bool chunk01 = !world->containsKey(mapPosition.x,     mapPosition.z + 1);
	bool chunk11 = !world->containsKey(mapPosition.x + 1, mapPosition.z + 1);

	int start = RENDER_DISTANCE * CHUNK_SIZE;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (i == CHUNK_SIZE - 1) {
				if (chunk10) break;
				if (k == CHUNK_SIZE - 1 && chunk11 && chunk01)
					break;
			}
			if (k == CHUNK_SIZE - 1) {
				if (chunk01) break;
			}

			double y = getHeightMap()[i][k];
			double x = i + pos.x;
			double z = k + pos.z;
			double zz = z +(i % 2 == 0 ? 0 : 0.5);
			double val = i % 2 == 0 ? 0.5 : 0;

			Vector4f min = getLeast(i, y, k);
			//mesh.addTriangles(generateTriangles(i, k, min.w), 0);
			mesh.addTriangles(generateTriangles(min.w));
			mesh.addVertex(x, y, zz);
			mesh.addVertex(outOfBoundsVertex(x, z + val, i, k, 1, 0));
			mesh.addVertex(outOfBoundsVertex(x, zz, i, k, 0, 1));
			mesh.addVertex(outOfBoundsVertex(x, z + val, i, k, 1, 1));
		}
	}
}

void Chunk::renderMesh() {
	mesh.clear();
	buildMeshData();
	
	mesh.recalculateNormals();
	mesh.updateMeshData();
}

void Chunk::printHeightMap() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		std::string sb = "\n";
		for (int k = 0; k < CHUNK_SIZE; k++) {
			sb.append(std::to_string(dheightMap[i][k]));
			sb.append(",\t");
		}
		sb.append("\t");
		std::cout << sb << std::endl;
		sb.clear();
	}
}

bool Chunk::readyForWaveUpdate() {
	if (!hasWater) return true;

	return ready;
}

void Chunk::updateMesh() {
	mesh.updateMeshData();
	canceled = true;
	stop = false;
	ready = false;
}

void Chunk::resetResumable() {
	canceled = true;
	stop = false;
	ready = false;
}

routine Chunk::yieldBuildMeshData() {
	Random::setSeedToCurrentTime();
	const Vector3f pos = transform.getPosition();

	bool chunk10 = !world->containsKey(mapPosition.x + 1, mapPosition.z);
	bool chunk01 = !world->containsKey(mapPosition.x, mapPosition.z + 1);
	bool chunk11 = !world->containsKey(mapPosition.x + 1, mapPosition.z + 1);

	MeshData meshData;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		co_yield 100ns;
		for (int k = 0; k < CHUNK_SIZE; k++) {
			//if (k % 8 == 0) co_yield 100ns;
			if (i == CHUNK_SIZE - 1) {
				if (chunk10) break;
				if (k == CHUNK_SIZE - 1 && chunk11 && chunk01)
					break;
			}
			if (k == CHUNK_SIZE - 1) {
				if (chunk01) break;
			}

			double y = getHeightMap()[i][k];
			double x = i + pos.x;
			double z = k + pos.z;
			double zz = z + (i % 2 == 0 ? 0 : 0.5);
			double val = i % 2 == 0 ? 0.5 : 0;

			Vector4f min = getLeast(i, y, k);
			meshData.addTriangles(generateTriangles(min.w));
			meshData.addVertex(x, y, zz);
			meshData.addVertex(outOfBoundsVertex(x, z + val, i, k, 1, 0));
			meshData.addVertex(outOfBoundsVertex(x, zz, i, k, 0, 1));
			meshData.addVertex(outOfBoundsVertex(x, z + val, i, k, 1, 1));
		}
	}
	mesh.setVertices(meshData.getVertices());
	mesh.setTriangles(meshData.getIndices());
}

float waveHeight = 0.2f;
float speed = 0.5f;
float waveLength = 1.0f;
float randomHeight = 0.2f;
float randomSpeed = 7.0f;

void Chunk::resumeWaves() {
	if (world->options.useWaves && hasWater) {
		if (canceled) {
			resumable = updateWaves();
			canceled = false;
		}
		else if (!stop) resumable.resume();

	}
}

routine Chunk::waveMotion() {

	const double waterMax = world->biomeOptions.waterMax;
	const double waterMin = world->biomeOptions.waterMin;

	Vector3f pos = transform.getPosition();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		yield 3ns;
		for (int k = 0; k < CHUNK_SIZE; k++) {

			double h = dheightMap[i][k];
			if (h < waterMax) {

				int x = i + pos.x;
				int z = k + pos.z;

				int seed = (int)(x*x + z*z);
				Random::setSeed(seed);

				double val0 = Random::Range(0.0, 0.3);
				double val1 = Random::Range(0.0, 0.3);
				h += sin(Time::getTime() * speed + x * waveLength + h * waveLength) * waveHeight;
				h += sin(cos(val0) * randomHeight * cos(Time::getTime() * randomSpeed * sin(val1)));
				if (h > waterMax) h = waterMax - Random::Range(0.0, 0.3);
				else if (h < waterMin) h = waterMin + Random::Range(0.0, 0.3);

				dheightMap[i][k] = h;
			}
		}
	}
	int i = 0;
	for (auto time : yieldBuildMeshData()) {
		if (i++ % 5 == 0) yield time;
	}
	mesh.recalculateNormals();

	//stop = true;
	ready = true;
	//canceled = true;
}




Resumable Chunk::updateWaves() {
	using namespace std;
	using namespace std::chrono;

	const double waterMax = world->biomeOptions.waterMax;
	const double waterMin = world->biomeOptions.waterMin;

	Array<Array<double>> hmap;
	hmap.resize(CHUNK_SIZE);

	// height map
	Vector3f pos = transform.getPosition();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		hmap[i].resize(CHUNK_SIZE);
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (k % 4 == 0) co_await suspend_always{};

			double h = dheightMap[i][k];

			if (h < waterMax) {

				int x = i + pos.x;
				int z = k + pos.z;


				int seed = (int)(x*x + z*z);
				Random::setSeed(seed);

				double val0 = Random::Range(0.0, 0.3);
				double val1 = Random::Range(0.0, 0.3);
				h += sin(Time::getTime() * speed + x * waveLength + h * waveLength) * waveHeight;
				h += sin(cos(val0) * randomHeight * cos(Time::getTime() * randomSpeed * sin(val1)));
				if (h > waterMax) h = waterMax - Random::Range(0.0, 0.3);
				else if (h < waterMin) h = waterMin + Random::Range(0.0, 0.3);
			}
			hmap[i][k] = h;
		}
	}
	co_await suspend_always{};

	dheightMap = hmap;
	int i = 0;
	for (auto v : yieldBuildMeshData()) {
		if (i++ % 16 == 0) co_await suspend_always{};
	}
	mesh.recalculateNormals();

	// update mesh is done in world.updateChunks()
	//mesh.updateMeshData();

	//canceled = true;
	stop = true;
	ready = true;
	co_await suspend_always {};
}



//future<void> Chunk::startRoutine() {
//	//updateWaves();
//}





std::vector<unsigned int> Chunk::generateTriangles(int i) {
	const static std::vector<unsigned int> _0 = { 0, 2, 1, 3, 1, 2 };
	const static std::vector<unsigned int> _1 = { 2, 3, 0, 1, 0, 3 };

	if (i == 1 || i == 2) 
		return _0;
	return _1;
}

std::vector<unsigned int> Chunk::generateTriangles(int i, int k, int min) {

	unsigned int _00 = (i << 4) + k;
	unsigned int _10 = _00 + 1;
	unsigned int _01 = ((i + 1) << 4) + k;
	unsigned int _11 = _01 + 1;

	if (i == 1 || i == 2) {
		return { _00, _10, _01, _11, _01, _10 };
	}
	return { _10, _11, _00, _01, _00, _11 };
}


bool Chunk::isOutOfBounds(int x, int y, int z) const {
	if (x >= CHUNK_SIZE   || x < 0) return true;
	if (y >= CHUNK_HEIGHT || y < 0) return true;
	if (z >= CHUNK_SIZE   || z < 0) return true;

	return false;
}

bool Chunk::isCell(int x, int y, int z) const {
	if (isOutOfBounds(x, y, z)) return false;
	if (cells[x][y][z] == Cell::Air) return false;
	if (cells[x][y][z].isNegligible()) return false;

	return true;
}

bool Chunk::isTransparentAt(int x, int y, int z) {
	return getCell(x, y, z).isTransparent();
}



void Chunk::setCell(int x, int y, int z, Cell const & cell) {
	if (isOutOfBounds(x, y, z)) return;

	cells[x][y][z] = cell;
}

Cell & Chunk::removeCell(int x, int y, int z) {
	if (isOutOfBounds(x, y, z)) return Cell::Air;

	Cell & cell = cells[x][y][z];
	cells[x][y][z] = Cell::Air;
	return cell;
}

Cell & Chunk::getCell(int x, int y, int z) {
	if (isOutOfBounds(x, y, z)) return Cell::Air;

	return cells[x][y][z];
}

Vector4f Chunk::getLeast(int i, int j, int k) {
	// crashes for some reason

	Vector4f min;
	min.y = getHeightMap()[i][k];
	min.w = 0;

	int count = 0;
	for (int ii = 0; ii < 2; ii++) {
		for (int kk = 0; kk < 2; kk++) {
			int oi = 0;
			int ok = 0;
			int _ii = 0;
			int _kk = 0;
			bool outOfBounds = false;

			// out of bounds in x coordinate
			if (isOutOfBounds(i + ii, 0, kk)) {
				outOfBounds = true;
				_kk = kk + k;
				oi = 1;
			}

			// out of bounds in z coordinate
			if (isOutOfBounds(i, 0, k + kk)) {
				_ii = ii + i;
				ok = 1;
				if (outOfBounds) {
					_ii = 0;
					_kk = 0;
				}

				outOfBounds = true;
			}
			//if (outOfBounds) {
			//	double y = world->getChunk(mapPosition.x + oi, mapPosition.z + ok).getHeightMap()[_ii][_kk];
			//	_kk += _ii % 2 == 0 ? 0.5 : 0;
			//	mesh.addVertex(transform.getPosition().x + _ii, y, transform.getPosition().z + _kk);
			//}

			int jj = outOfBounds 
				? world->getChunk(mapPosition.x + oi, mapPosition.z + ok)
						.getHeightMap()[_ii][_kk]
				: getHeightMap()[ii+i][kk+k];
			if (min.y > jj) {
				min.x = ii;
				min.y = jj;
				min.z = kk;
				min.w = count;
			}
			count++;
		}
	}
	return min;
}

Chunk & Chunk::getNeighbor(int x, int z) {
	std::string key = world->toKey(mapPosition.x + x, mapPosition.z + z);
	if (world->containsKey(key)) {

	}
	return *Chunk::EMPTY;
}

void Chunk::clear() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			getHeightMap()[i][k] = 0;
			//for (int j = 0; j < CHUNK_HEIGHT; j++) {
			//	cells[i][j][k] = Cell::Air;
			//}
		}
	}
}




bool Chunk::isInvalid() const {
	return empty;
}

Array<Array<double>> & Chunk::getHeightMap() {
	return dheightMap;
}


Chunk & Chunk::setMapPosition(Vector3f const & val) {
	mapPosition = val;
	return *this;
}



void Chunk::clearAllObjects() {
	clearBuildings();
	clearPlants();
}

void Chunk::clearBuildings() {
	buildings.clear();
}

void Chunk::clearPlants() {
	plants.clear();
}



Chunk::Chunk(bool _empty) : empty(_empty), timer(1.0f) {}
Chunk::Chunk(World * _world) : world(_world), timer(1.0f) {}