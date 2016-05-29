#pragma once

#include "Commons.h"
#include "Cell.h"
#include "TransformEntity.h"

#define CHUNK_SIZE 8

template <class T>
using Array = std::vector<T>;

class Chunk : public TransformEntity {
private:
	Array<Array<int>> heightMap;
	Array<Array<Array<Cell>>> cells;

public: /* Lifecycle */
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	void generateCells();
	void renderMesh();




	void onDestroy() override;
};