#pragma once

#include "Commons.h"
#include "Cell.h"
#include "Component.h"
#include "Mesh.h"

#define CHUNK_SIZE 8

template <class T>
using Array = std::vector<T>;

class Chunk : public BaseEntity {
public:
	Transform transform;

private:
	Mesh mesh;
	Array<Array<int>> heightMap;
	Array<Array<Array<Cell>>> cells;


public: /* Lifecycle */
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	void loadToShader();
	void generateChunk();
	void buildMesh();
	void renderMesh();
	void printHeightMap();

	bool isOutOfBounds(int x, int y, int z) const;
	bool isCell(int x, int y, int z) const;
	Cell& getCell(int x, int y, int z);
	std::vector<Vector4f> getSequence(int i, int j, int k);
	Vector4f getLeast(int i, int j, int k);
	std::vector<unsigned int> generateTriangles(int i);

	void onDestroy() override;
};