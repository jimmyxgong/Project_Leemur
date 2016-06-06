#pragma once

#include "Commons.h"
#include "Cell.h"
#include "Component.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Time.h"
#include "AsyncTask.h"
#include "Resumable.h"
#include "MeshData.h"
#include "Building.h"

#ifdef _WIN32
#include <experimental/generator>
#include <future>
#endif

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 64
#define HEIGHT_UNIT 0.35
#define HEIGHT_CONSTANT CHUNK_HEIGHT


using namespace std::experimental;
template <class T>
using Array = std::vector<T>;

class World;

class Chunk : public BaseEntity {
public:
	static UniquePointer<Chunk> EMPTY;
	Transform transform;

private:
	Array<SharedPointer<Building>> buildings;
	Array<SharedPointer<Transform>> plants;

	bool ready = false;
	bool empty = false;
	bool stop = false;
	Mesh mesh;
	Mesh waveMesh;
	//AsyncTask task;
	std::atomic<bool> canceled = true;
	std::atomic<bool> hasWater;
	Resumable resumable;
	//future<void> future;

	//Array<Array<int>> interpolatedHeightMap;
	Array<Array<int>> heightMap;
	Array<Array<double>> dheightMap;

	/* Unused for this project */
	Array<Array<Array<Cell>>> cells;
	bool changed = false;

	/* Parent of chunk */
	World * world;

	/* Position in chunk map */
	Vector3f mapPosition;
	Time timer;


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
	void generateChunk(Terrain & terrain);
	void buildMeshData();
	void renderMesh();
	void printHeightMap();
	
	
	/* Methods used for wave motion*/
	bool readyForWaveUpdate();
	void updateMesh();
	void resetResumable();
	void resumeWaves();

	routine waveMotion();
	routine yieldBuildMeshData();
    Resumable updateWaves();

public: /* Info functions */
	std::vector<unsigned int> generateTriangles(int i);
	std::vector<unsigned int> generateTriangles(int i, int k, int min);

	bool isOutOfBounds(int x, int y, int z) const;
	bool isCell(int x, int y, int z) const;
	bool isTransparentAt(int x, int y, int z);
	
	
	void setCell(int x, int y, int z, Cell const & cell);
	Cell & removeCell(int x, int y, int z);
	Cell& getCell(int x, int y, int z);
	Vector4f getLeast(int i, int j, int k);
	
	Chunk & getNeighbor(int x, int z);
	Vector3f outOfBoundsVertex(double x, double z, int fi, int fk, int i, int k);

	void clear();

public:
	bool isInvalid() const;
	Array<Array<double>> & getHeightMap();
	Chunk & setMapPosition(Vector3f const &);

public: /* object functions */
	void clearAllObjects();
	void clearBuildings();
	void clearPlants();

public:
	Chunk(bool empty = false);
	Chunk(World * world = nullptr);
};