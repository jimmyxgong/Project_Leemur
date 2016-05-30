#include "Chunk.h"
#include "Terrain.h"
#include "SolidCell.h"

#define PARTS 3.2
#define DIV 0.7
#define CUTOFF 5.3

Terrain terrain = {
	6.7, 0.6, 1.0, 8, 1 
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

	

	// Allocate memory to the 3D Grid of cells
	//cells = new Cell**[CHUNK_SIZE];
	//for (int i = 0; i < CHUNK_SIZE; i++) {
	//	cells[i] = new Cell*[CHUNK_SIZE];
	//	for (int j = 0; j < CHUNK_SIZE; j++) {
	//		cells[i][j] = new Cell[CHUNK_SIZE];
	//	}
	//}
}

void Chunk::onStart() {
	generateChunk();
	printHeightMap();
}

void Chunk::onRender() {

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


void Chunk::onDestroy() {
	BaseEntity::onDestroy();



	//for (int i = 0; i < CHUNK_SIZE; i++) {
	//	for (int j = 0; j < CHUNK_SIZE; j++) {
	//		delete[] cells[i][j];
	//	}
	//	delete[] cells[i];
	//}
	//delete[] cells;

}