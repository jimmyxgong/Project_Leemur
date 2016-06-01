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

#define PARTS 1.2 // 3.2
#define DIV 4.3	// 0.7
#define CUTOFF 3.3 // 5.3

//double _parts = 3.2;
//double _div = 0.7;
//double _cutoff = 5.3;

double _parts = 10.2;
double _div = 20.7;
double _cutoff = 9.8;

// cutoff div = -33 parts 1.2

// persist 7.2
Terrain terrain = {
	0.25, 1, 1, 6, 1		//6.7, 0.6, 1.0, 8, 1 
};
// parts 10.2 div 20.7 cutoff 9.8

Keyboard::Layout bindings;


UniquePointer<Chunk> Chunk::EMPTY = unique<Chunk>();


void print(double val) {
	std::cout << (std::to_string(val)) << std::endl;
}

void Chunk::onStart() {
	resizeStructure();
	//testBindings();

	generateChunk();
	//printHeightMap();
	buildMeshData();

	mesh.recalculateNormals();
	mesh.init();
}

void Chunk::onRender() {
	Resources::getShader(SHADER_LIGHT).use();
	Light::Directional.loadToShader();
	Material::Gold.loadToShader();
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

void Chunk::allowKeyBindings() {
	// Testing values
	bindings.onKeyPressed(GLFW_KEY_P, [this](bool shifted) {
		this->changed = true;
		_parts += shifted ? -0.2 : 0.2;
		printf("parts");
		print(_parts);
	});

	bindings.onKeyPressed(GLFW_KEY_O, [this](bool shifted) {
		this->changed = true;
		_div += shifted ? -0.2 : 0.2;
		printf("div");
		print(_div);
	});

	bindings.onKeyPressed(GLFW_KEY_I, [this](bool shifted) {
		changed = true;
		_cutoff += shifted ? -0.2 : 0.2;
		printf("cutoff");
		print(_cutoff);
	});

	bindings.onKeyPressed(GLFW_KEY_J, [this](bool shifted) {
		changed = true;
		terrain.persistence += shifted ? -0.2 : 0.2;
		printf("persist");
		print(terrain.persistence);
	});

	bindings.onKeyPressed(GLFW_KEY_K, [this](bool shifted) {
		changed = true;
		terrain.frequency += shifted ? -0.2 : 0.2;

		printf("freq");
		print(terrain.frequency);
	});

	bindings.onKeyPressed(GLFW_KEY_L, [this](bool shifted) {
		changed = true;
		terrain.amplitude += shifted ? -0.2 : 0.2;

		printf("amp");
		print(terrain.amplitude);
	});

	bindings.onKeyPressed(GLFW_KEY_H, [this](bool shifted) {
		changed = true;
		terrain.octaves += shifted ? -1 : 1;
		printf("Oct");
		print(terrain.octaves);
	});

	Keyboard::addLayout(&bindings);
}

void Chunk::resizeStructure() {
	cells.resize(CHUNK_SIZE);
	heightMap.resize(CHUNK_SIZE);
	//interpolatedHeightMap.resize(CHUNK_SIZE);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		cells[i].resize(CHUNK_HEIGHT);
		heightMap[i].resize(CHUNK_SIZE);
		//interpolatedHeightMap.resize(CHUNK_SIZE);
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			cells[i][j].resize(CHUNK_SIZE);
		}
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

void Chunk::generateChunk() {
	clear();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				Vector3f pos = transform.getPosition();
				double x = i + round(pos.x);
				double y = j;
				double z = k + round(pos.z);
				//x /= 10.0;
				//y /= 10.0;
				//z /= 10.0;


				double height = terrain.perlinNoise(x, y, z, _parts, _div);
					//height = terrain.height(x, z);
				if (j > 0 && height > _cutoff) {
					cells[i][j][k] = Cell::Air;
					continue;
				}

				if (heightMap[i][k] < j)
					heightMap[i][k] = j;

				cells[i][j][k] = SolidCell();
			}
		}
	}
}

void Chunk::buildMeshData() {

	const Vector3f pos = transform.getPosition();
	const double d = HEIGHT_UNIT;

	for (int i = 0; i < CHUNK_SIZE - 1; i++) {
		for (int k = 0; k < CHUNK_SIZE - 1; k++) {
			int j = heightMap[i][k];
			Vector4f min = getLeast(i, j, k);

			mesh.addTriangles(generateTriangles(min.w));
			double x = i + pos.x;
			double z = k + pos.z;

			mesh.addVertex(x, j * d, z);
			mesh.addVertex(x + 1, heightMap[i + 1][k] * d, z);
			mesh.addVertex(x, heightMap[i][k + 1] * d, z + 1);
			mesh.addVertex(x + 1, heightMap[i + 1][k + 1] * d, z + 1);
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
		std::string sb = "{\t";
		for (int k = 0; k < CHUNK_SIZE; k++) {
			sb.append(std::to_string(heightMap[i][k]));
			sb.append(",\t");
		}
		sb.append("\t}");
		std::cout << sb << std::endl;
		sb.clear();
	}
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

bool Chunk::isOutOfBounds(int x, int y, int z) const {
	if (x >= CHUNK_SIZE || x < 0) return true;
	if (y >= CHUNK_HEIGHT || y < 0) return true;
	if (z >= CHUNK_SIZE || z < 0) return true;

	return false;
}

bool Chunk::isCell(int x, int y, int z) const {
	if (isOutOfBounds(x, y, z)) return false;
	if (cells[x][y][z] == Cell::Air) return false;
	if (cells[x][y][z].isNegligible()) return false;

	return true;
}

Cell& Chunk::getCell(int x, int y, int z) {
	if (isOutOfBounds(x, y, z)) return Cell::Air;

	return cells[x][y][z];
}

Vector4f Chunk::getLeast(int i, int j, int k) {
	Vector4f min(0.0f);
	int count = 0;
	for (int ii = 0; ii < 2; ii++) {
		for (int kk = 0; kk < 2; kk++) {
			int jj = heightMap[ii][kk];
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

std::vector<unsigned int> Chunk::generateTriangles(int i) {
	//const static std::vector<unsigned int> TYPE0 = { 0, 2, 1, 3, 1, 2 };
	if (i == 0 || i == 1) return{ 0, 2, 1, 3, 1, 2 };
	
	return{ 0, 1, 2, 3, 2, 1 };
}

bool Chunk::isTransparentAt(int x, int y, int z) {
	return getCell(x, y, z).isTransparent();
}


void Chunk::clear() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			heightMap[i][k] = 0;
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				cells[i][j][k] = Cell::Air;
			}
		}
	}
}

void Chunk::onDestroy() {
	BaseEntity::onDestroy();
	mesh.destroy();
}


Chunk::Chunk(World * _world) : world(_world) {}