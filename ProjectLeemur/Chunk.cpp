#include "Chunk.h"
#include "Terrain.h"
#include "SolidCell.h"
#include "Resources.h"
#include "Light.h"
#include "Window.h"
#include "Resources.h"
#include "Camera.h"
#include "Keyboard.h"

#define PARTS 1.2 // 3.2
#define DIV 4.3	// 0.7
#define CUTOFF 3.3 // 5.3

double _parts = 1.2;
double _div = 0.5;
double _cutoff = 0.3;

// cutoff div = -33 parts 1.2

// persist 7.2
Terrain terrain = {
	6.6, 0.6, 1.0, 2, 1		//6.7, 0.6, 1.0, 8, 1 
};

Keyboard::Layout bindings;

void print(std::string const & val) {
	std::cout << val << std::endl;
}

void print(double val) {
	print(std::to_string(val));
}

void Chunk::onCreate() {
	cells.resize(CHUNK_SIZE);
	heightMap.resize(CHUNK_SIZE);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		cells[i].resize(CHUNK_SIZE);
		heightMap[i].resize(CHUNK_SIZE);
		for (int j = 0; j < CHUNK_SIZE; j++) {
			cells[i][j].resize(CHUNK_SIZE);
		}
	}

	bindings.onKeyPressed(GLFW_KEY_P, [this](bool shifted) {
		this->changed = true;
		_parts += shifted ? 1 : -1;
		print("parts");
		print(_parts);
	});
	bindings.onKeyPressed(GLFW_KEY_O, [this](bool shifted) {
		this->changed = true;
		_div += shifted ? 1 : -1;
		print("div");
		print(_div);
	});
	bindings.onKeyPressed(GLFW_KEY_I, [this](bool shifted) {
		changed = true;
		_cutoff += shifted ? 1 : -1;
		print("cutoff");
		print(_cutoff);
	});

	bindings.onKeyPressed(GLFW_KEY_J, [this](bool shifted) {
		changed = true;
		terrain.persistence += shifted ? -0.2 : 0.2;
		print("persist");
		print(terrain.persistence);
	});

	bindings.onKeyPressed(GLFW_KEY_K, [this](bool shifted) {
		changed = true;
		terrain.frequency += shifted ? -0.2 : 0.2;

		print("freq");
		print(terrain.frequency);
	});

	bindings.onKeyPressed(GLFW_KEY_L, [this](bool shifted) {
		changed = true;
		terrain.amplitude += shifted ? -0.2 : 0.2;

		print("amp");
		print(terrain.amplitude);
	});

	bindings.onKeyPressed(GLFW_KEY_H, [this](bool shifted) {
		changed = true;
		terrain.octaves += shifted ? -1 : 1;
		print("Oct");
		print(terrain.octaves);
	});

	Keyboard::addLayout(&bindings);
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


void Chunk::onStart() {
	generateChunk();
	printHeightMap();
	//buildMeshData();

	//mesh.recalculateNormals();
	//mesh.onStart();
}

void Chunk::onRender() {
	Resources::getShader(SHADER_LIGHT).use();
	Light::Directional.loadToShader();
	Material::Gold.loadToShader();
	loadToShader();
	mesh.onRender();
}

void Chunk::onUpdate() {
	if (changed) {
		print("Updating");
		generateChunk();
		printHeightMap();
		//renderMesh();
		changed = false;
	}
}

void Chunk::generateChunk() {
	clear();
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				double x = i + round(transform.getPosition().x);
				double z = k + round(transform.getPosition().z);

				double height = terrain.height(x, z);
				if (j > 1 && height > _cutoff) {
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
	for (int i = 0; i < CHUNK_SIZE - 1; i++) {
		for (int k = 0; k < CHUNK_SIZE - 1; k++) {
			int j = heightMap[i][k];
			Vector4f min = getLeast(i, j, k);

			mesh.addTriangles(generateTriangles(min.w));

			const int d = 1;
			mesh.addVertex(i, j / d, k);
			mesh.addVertex(i + 1, heightMap[i + 1][k] / d, k);
			mesh.addVertex(i, heightMap[i][k + 1] / d, k + 1);
			mesh.addVertex(i + 1, heightMap[i + 1][k + 1] / d, k + 1);
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
		std::string sb = "{ ";
		for (int k = 0; k < CHUNK_SIZE; k++) {
			sb.append(std::to_string(heightMap[i][k]));
			sb.append(", ");
		}
		sb.append(" }");
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
	if (y >= CHUNK_SIZE || y < 0) return true;
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


void Chunk::clear() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			heightMap[i][k] = 0;
			for (int j = 0; j < CHUNK_SIZE; j++) {
				cells[i][j][k] = Cell::Air;
			}
		}
	}
}

void Chunk::onDestroy() {
	BaseEntity::onDestroy();
	mesh.onDestroy();
}