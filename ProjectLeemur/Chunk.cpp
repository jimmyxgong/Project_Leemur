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

#define PARTS 1.2 // 3.2
#define DIV 4.3	// 0.7
#define CUTOFF 3.3 // 5.3

//double _parts = 3.2;
//double _div = 0.7;
//double _cutoff = 5.3;

//double _parts = 9.8;
//double _div = 1.5;
//double _cutoff = 10.7;

//double _parts = 10.2;
//double _div = 20.7;
//double _cutoff = 9.8;

// cutoff div = -33 parts 1.2

// persist 7.2
//Terrain terrain = {
//	0.25, 1, 1, 6, 100	//6.7, 0.6, 1.0, 8, 1 
//}; //0.05, 1, 1, 6, 100
//// parts 10.2 div 20.7 cutoff 9.8
//
//Keyboard::Layout bindings;


UniquePointer<Chunk> Chunk::EMPTY = unique<Chunk>(true);


void print(double val) {
	std::cout << (std::to_string(val)) << std::endl;
}



void Chunk::onCreate() {
	resizeStructure();
	generateChunk();
	//printHeightMap();
}

void Chunk::onStart() {
	buildMeshData();

	mesh.recalculateNormals();
	mesh.init();
}

void Chunk::onRender() {
	loadToShader();
	mesh.render();
}

void Chunk::onUpdate() {
	if (changed) {
		printf("Updating");
		generateChunk();
		//printHeightMap();
		renderMesh();
		changed = false;
	}
}

void Chunk::onDestroy() {
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
	//double val = terrain.fbm(0, 0, 0);
	//printf("val: %.4f", val);

	Random::setSeedToCurrentTime();

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
			if (h < 2.22) {
				h = 2.199;
				h -= Random::Range(0.0, 0.3);
			}
			dheightMap[i][k] = h;

			//dheightMap[i][k] = g;
			//heightMap[i][k] = (int)round(g * HEIGHT_CONSTANT);
		}
	}
}





















void Chunk::generateChunk() {
	clear();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		//for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				Vector3f pos = transform.getPosition();
				double x = i + round(pos.x);
				double z = k + round(pos.z);
				
				//int j = CHUNK_HEIGHT;
				//double height = terrain.perlinNoise(x, j, z, _parts, _div);
				//while (height > _cutoff && j > 0) {
				//	height = terrain.perlinNoise(x, --j, z, _parts, _div);
				//}
				//height = j;
					//height = terrain.height(x, z);
				//if (j > 0 && height > _cutoff) {
				//	//cells[i][j][k] = Cell::Air;
				//	continue;
				//}

				//if (heightMap[i][k] < j)
					//heightMap[i][k] = j;

				//cells[i][j][k] = SolidCell();
				//double g = terrain.height(x, z);
				//heightMap[i][k] = (int) round(g); // (int)((g - (int)g) * 10);
			}
		//}
	}
}

// When there are two chunks: we need to interwove their vertices together
// while also maintaining their out of bounds checking.
void Chunk::addMeshOutOfBounds(double x, double z, int fi, int fk, int i, int k) {
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
		Chunk const & chunk = world->getChunk(key);
		if (chunk.isInvalid()) {
			printf("INVALID chunk found in out of bounds\n");
			printf("at %d, %d", i, k);
			return;
		}
		//Array<Array<double> const & map = world->getChunk(key).getHeightMap();
		//mesh.addVertex(x + i, map[ii][kk] * d, z + k);
		//return;
		y = world->getChunk(key).getHeightMap()[ii][kk];
	}
	else y = getHeightMap()[fi + i][fk + k];

	mesh.addVertex(x + i, y, z + k);
}

void Chunk::buildMeshData() {
	Random::setSeedToCurrentTime();

	const Vector3f pos = transform.getPosition();
	const double d = HEIGHT_UNIT;

	bool chunk10 = !world->containsKey(mapPosition.x + 1, mapPosition.z);
	bool chunk01 = !world->containsKey(mapPosition.x,     mapPosition.z + 1);
	bool chunk11 = !world->containsKey(mapPosition.x + 1, mapPosition.z + 1);

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
			mesh.addTriangles(generateTriangles(min.w));

			mesh.addVertex(x, y, zz);
			addMeshOutOfBounds(x, z + val, i, k, 1, 0);
			addMeshOutOfBounds(x, zz, i, k, 0, 1);
			addMeshOutOfBounds(x, z + val, i, k, 1, 1);
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

std::vector<unsigned int> Chunk::generateTriangles(int i) {
	const static std::vector<unsigned int> _0 = { 0, 2, 1, 3, 1, 2 };
	const static std::vector<unsigned int> _1 = { 2, 3, 0, 1, 0, 3 };

	if (i == 1 || i == 2) 
		return _0;
	return _1;
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



Chunk::Chunk(bool _empty) : empty(_empty) {}
Chunk::Chunk(World * _world) : world(_world) {}