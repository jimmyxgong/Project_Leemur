#include "Chunk.h"
#include "Terrain.h"
#include "SolidCell.h"
#include "Resources.h"
#include "Light.h"
#include "Window.h"
#include "Resources.h"
#include "Camera.h"

#define PARTS 10.1 // 3.2
#define DIV 2.4	// 0.7
#define CUTOFF 1.3 // 5.3

Terrain terrain = {
	100.1, 0.6, 1.0, 8, 1		//6.7, 0.6, 1.0, 8, 1 
};


//UniquePointer<Cell> createCell(unsigned int type) {
//	UniquePointer<Cell> cell = unique<Cell>();
//
//	cell->data.type = type;
//
//	return cell;
//}

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
	Shader::loadVector("CameraPosition", CAMERA_POSITION);
}


void Chunk::onStart() {
	generateChunk();
	printHeightMap();

	for (int i = 0; i < CHUNK_SIZE - 1; i++) {
		for (int k = 0; k < CHUNK_SIZE - 1; k++) {
			int j = heightMap[i][k];
			Vector4f min = getLeast(i, j, k);

			mesh.addTriangles(generateTriangles(min.w));

			mesh.addVertex(i, j, k);
			mesh.addVertex(i + 1, heightMap[i + 1][k], k);
			mesh.addVertex(i, heightMap[i][k + 1], k + 1);
			mesh.addVertex(i + 1, heightMap[i + 1][k + 1], k + 1);
		}
	}
	//for (int i = 0; i < 10; i++) {
	//	std::cout << mesh.getIndices().at(i) << std::endl;
	//}
	//ObjObject & pod = (ObjObject&)Resources::getEntity(POD_OBJ);
	//mesh.setVertices()

	//mesh.addTriangles({0, 1, 2});
	//mesh.addVertex(0.951057, -1, -0.309017);
	//mesh.addVertex(0.809018, -1, -0.587786);
	//mesh.addVertex(-0.951057, -1, 0.309017);

	//mesh.addNormal(2.951049, -2.827435, -0.958847);
	//mesh.addNormal(2.510310, -2.827429, -1.823846);
	//mesh.addNormal(-2.951046, -2.827430, 0.958853);


	//mesh.setVertices(pod.getVertices());
	//mesh.setNormals(pod.getNormals());
	//mesh.setTriangles(pod.getIndices());


	mesh.recalculateNormals();

	//for (int i = 0; i < 3; i++) {
	//	std::cout << to_string(mesh.getNormals().at(i)) << std::endl;
	//}
	mesh.onStart();
}

void Chunk::onRender() {
	Resources::getShader(SHADER_LIGHT).use();
	Light::Directional.loadToShader();
	Material::Gold.loadToShader();
	loadToShader();
	mesh.onRender();
}

void Chunk::onUpdate() {
	
}

void Chunk::generateChunk() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				double x = i + round(transform.getPosition().x);
				double z = k + round(transform.getPosition().z);

				if (j > 1 && terrain.perlinNoise(x, j, z, PARTS, DIV) > CUTOFF) {
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

void Chunk::renderMesh() {

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

//std::vector<Vector4f> Chunk::getSequence(int i, int j, int k) {
//	Vector4f min(-1.f);
//	Vector4f max(-1.f);
//	Vector4f mid0(-1.f);
//	Vector4f mid1(-1.f);
//	std::vector<Vector4f> list;
//
//	int count = 0;
//	for (int ii = 0; ii < 2; ii++) {
//		for (int kk = 0; kk < 2; kk++) {
//			int jj = heightMap[ii][kk];
//			if (min.y > jj) {
//				if (mid0.x == -1.f)
//					mid0 = max;
//				else mid1 = max;
//
//				min.x = ii;
//				min.y = jj;
//				min.z = kk;
//				min.w = count;
//			}
//			else if (max.y > jj) {
//				if (mid0.x == -1.f)
//					mid0 = max;
//				else mid1 = max;
//
//				max.x = ii;
//				max.y = jj;
//				max.z = kk;
//				max.w = count;
//			}
//			else 
//			{
//				if (mid0.x == -1.f) {
//					mid0.x = ii;
//					mid0.y = jj;
//					mid0.z = kk;
//					mid0.w = count;
//				}
//				else
//				{
//					mid1.x = ii;
//					mid1.y = jj;
//					mid1.z = kk;
//					mid1.w = count;
//				}
//			}
//			count++;
//		}
//	}
//	list.push_back(min);
//	list.push_back(mid0);
//	list.push_back(mid1);
//	list.push_back(max);
//	
//	return list;
//}

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

void Chunk::onDestroy() {
	BaseEntity::onDestroy();
	mesh.onDestroy();
}