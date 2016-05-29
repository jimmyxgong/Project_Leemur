#include "Chunk.h"


UniquePointer<Cell> createCell(unsigned int type) {
	UniquePointer<Cell> cell = unique<Cell>();

	cell->data.type = type;

	return cell;
}

void Chunk::onCreate() {
	cells.resize(CHUNK_SIZE);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		cells[i].resize(CHUNK_SIZE);
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
	
}

void Chunk::onRender() {

}

void Chunk::onUpdate() {

}



void Chunk::generateCells() {

}

void Chunk::renderMesh() {

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