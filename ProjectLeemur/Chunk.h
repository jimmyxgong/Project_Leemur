#pragma once

#include "Commons.h"
#include "Cell.h"
#include "Component.h"
#include "Mesh.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 32
#define HEIGHT_UNIT 0.25

template <class T>
using Array = std::vector<T>;

class World;

class Chunk : public BaseEntity {
public:
	static UniquePointer<Chunk> EMPTY;
	Transform transform;

private:
	bool empty = false;
	Mesh mesh;

	//Array<Array<int>> interpolatedHeightMap;
	Array<Array<int>> heightMap;
	Array<Array<Array<Cell>>> cells;
	bool changed = false;

	World * world;

	// Position in chunk map
	Vector3f mapPosition;



public: /* Lifecycle */
	void onCreate() override;
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
	std::vector<unsigned int> generateTriangles(int i);

	bool isOutOfBounds(int x, int y, int z) const;
	bool isCell(int x, int y, int z) const;
	bool isTransparentAt(int x, int y, int z);
	
	
	void setCell(int x, int y, int z, Cell const & cell);
	Cell & removeCell(int x, int y, int z);
	Cell& getCell(int x, int y, int z);
	Vector4f getLeast(int i, int j, int k);
	
	Chunk & getNeighbor(int x, int z);
	void addMeshOutOfBounds(double x, double z, int fi, int fk, int i, int k);

	void clear();

public:
	bool isInvalid() const;
	Array<Array<int>> & getHeightMap();
	Chunk & setPosition(Vector3f const &);

public:
	Chunk(bool empty = false);
	Chunk(World * world = nullptr);
};