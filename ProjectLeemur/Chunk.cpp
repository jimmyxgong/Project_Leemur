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

#ifdef _WIN32
#include <future>
#include <pplawait.h>
#endif

#define PARTS 1.2 // 3.2
#define DIV 4.3	// 0.7
#define CUTOFF 3.3 // 5.3

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
}

void Chunk::onUpdate() {
	resumeWaves();


	/*for (auto c : updateWaves()) {
		printf("val %d", c.count());
	}*/
	//routine i = updateWaves();
	//for (auto c : i) {
	//	
	//}
	//co_await updateWaves();
	//coroutine_handle<int> handle;
	
	//if (!future.valid()) {
	//	canceled = false;
	//	future = async(
	//		std::launch::async,
	//		[this] {});
	//}
	//else {
	//	auto status = future.wait_for(0ms);
	//	if (status == std::future_status::ready) {
	//		canceled = false;
	//		future = async(
	//			std::launch::async,
	//			[this] {
	//			for (auto c : updateWaves()) {
	//				if (canceled) break;
	//				//printf("running at: %d", c);
	//				this_thread::sleep_for(1ms);
	//			}
	//			canceled = true;
	//		});
	//	}
	//}
	//if (!task.isValid()) {
		//task.StartCoroutine(updateWaves());
	//}
	//std::function<routine()> f = std::bind(&this->updateWaves);

	//if (canceled) {
	//	canceled = false;
	//	async(std::launch::async, [this] {
	//		for (auto time : updateWaves()) {
	//			if (canceled) break;
	//			//printf("running at: %d", c);
	//			this_thread::sleep_for(time);
	//		}
	//		canceled = true;
	//	});
	//}

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
	//cells.resize(CHUNK_SIZE);
	heightMap.resize(CHUNK_SIZE);
	dheightMap.resize(CHUNK_SIZE);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		//cells[i].resize(CHUNK_HEIGHT);
		heightMap[i].resize(CHUNK_SIZE);
		dheightMap[i].resize(CHUNK_SIZE);
		//for (int j = 0; j < CHUNK_HEIGHT; j++) {
			//cells[i][j].resize(CHUNK_SIZE);
		//}
	}
}

void Chunk::loadToShader() {
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
	Random::setSeedToCurrentTime();
	hasWater = false;
	clear();

	const Vector3f pos = transform.getPosition();
	const double roundedX = round(pos.x);// +pos.x < 0 ? 1000 : 999;
	const double roundedZ = round(pos.z);// +pos.z < 0 ? 1000 : 999;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		double x = i + roundedX;
		for (int k = 0; k < CHUNK_SIZE; k++) {
			double z = k + roundedZ;

			double g = terrain.height(x, z);
			double h = ((int) round(g * HEIGHT_CONSTANT)) * HEIGHT_UNIT;
			if (h < world->biomeOptions.waterMax + 0.02) {
				h = world->biomeOptions.waterMax - 0.01;
				h -= Random::Range(0.0, 0.3);
				hasWater = true;
			}
			dheightMap[i][k] = h;

			//dheightMap[i][k] = g;
			//heightMap[i][k] = (int)round(g * HEIGHT_CONSTANT);
		}
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
	for (int i = 0; i < CHUNK_SIZE; i++) {
		double displace = i % 2 == 0 ? 0 : 0.5;
		for (int k = 0; k < CHUNK_SIZE; k++) {
			double y = getHeightMap()[i][k];
			double x = i + pos.x;
			double z = k + pos.z;
			mesh.addVertex(x, y, z + displace);
		}
	}

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
			if (k % 8 == 0) co_yield 100ns;
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
		//if (canceled) {
		//	canceled = false;
		//	ready = false;
		//	async(std::launch::async, [this] {
		//		for (auto time : waveMotion()) {
		//			if (canceled) break;
		//			this_thread::sleep_for(time);
		//		}
		//	});
		//}
		if (canceled) {
			resumable = updateWaves();
			canceled = false;
		}
		else if (!stop) resumable.resume();

	}
	//else if (resumable.coroutine) {
	//	resumable.coroutine.destroy();
	//}
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
	//mesh.clear();
	int i = 0;
	for (auto time : yieldBuildMeshData()) {
		if (i++ % 16 == 0) yield time;
	}
	mesh.recalculateNormals();
	//mesh.updateMeshData();

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



Chunk::Chunk(bool _empty) : empty(_empty), timer(1.0f) {}
Chunk::Chunk(World * _world) : world(_world), timer(1.0f) {}