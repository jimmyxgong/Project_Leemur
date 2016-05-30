#pragma once

#include "Commons.h"
#include "Cell.h"
#include "Component.h"

#define CHUNK_SIZE 8

template <class T>
using Array = std::vector<T>;

class Chunk : public Component {
public:
	Transform transform;

private:
	Array<Array<int>> heightMap;
	Array<Array<Array<Cell>>> cells;


public: /* Lifecycle */
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	void generateChunk();
	void renderMesh();
	void printHeightMap();

	void onDestroy() override;
};