#pragma once

#include "Commons.h"
#include "Cell.h"
#include "Component.h"
#include "Mesh.h"

#define CHUNK_SIZE 8
#define CHUNK_HEIGHT 24
#define HEIGHT_UNIT 0.2

template <class T>
using Array = std::vector<T>;

class World;

class Chunk : public BaseEntity {
public:
	static UniquePointer<Chunk> EMPTY;
	Transform transform;

private:
	Mesh mesh;

	Array<Array<int>> interpolatedHeightMap;
	Array<Array<int>> heightMap;
	Array<Array<Array<Cell>>> cells;
	bool changed = false;

	World * world;
	Vector3f position;

public: /* Lifecycle */
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

public: /* terrain generations */
	void allowKeyBindings();
	void resizeStructure();
	void loadToShader();
	void generateChunk();
	void buildMeshData();
	void renderMesh();
	void printHeightMap();

public: /* Info functions */
	bool isOutOfBounds(int x, int y, int z) const;
	bool isCell(int x, int y, int z) const;
	
	
	
	void setCell(int x, int y, int z, Cell const & cell);
	Cell & removeCell(int x, int y, int z);


	Cell& getCell(int x, int y, int z);
	Vector4f getLeast(int i, int j, int k);
	
	std::vector<unsigned int> generateTriangles(int i);
	
	bool isTransparentAt(int x, int y, int z);

	void clear();

	Chunk(World * world = nullptr);
};