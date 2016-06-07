#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Chunk.h"
#include "Transform.h"
#include "Player.h"
#include "Terrain.h"

#ifdef _WIN32
#include <boost/unordered_map.hpp>
#endif

#define RENDER_DISTANCE 6
#define PLANT_BUILDING_FREQ_RENDER 0.0625

class World : public BaseEntity {
public:
	struct Options {
		bool useWaves = false;
		bool generateBuildings = true;
		bool generatePlants = true;

		bool isWaterDecreasing = false;

	} options;

	struct Biomes {
		float waterMax = 2.2;
		float waterMin = 1.9;

		//float grass = 4;
		float sand = 3.2;
		float snow = 6.7;

		float frequency = PLANT_BUILDING_FREQ_RENDER;
		float plantMin = sand + 0.5;
		float plantMax = snow - 0.6;

		float buildingMin = plantMax - 0.3;
		float buildingMax = snow + 1.2;

		bool restructureParts = false;
	} biomeOptions;


private:
	WeakPointer<Player> player;
	std::unordered_map<std::string, UniquePointer<Chunk>> chunks;

	/* Noise profile */
	Terrain terrain;
	bool changed = false;

	void allowKeyBindings();

public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

	void generateChunks();
	void evaluateChunks();
	
	void startChunks();
	void renderChunks();
	void updateChunks();
	void deleteChunks();
	
	void setNewChunk(int x, int z);
	void addNewChunk(int x, int z);
	void addNewChunk(Vector3f & chunkPos, int x, int z);
	std::string toKey(int x, int z);

	bool containsKey(int x, int z);
	bool containsKey(std::string const & key);

	Chunk & getChunk(int x, int z);
	Chunk & getChunk(std::string const & key);
	Vector3f getChunkPosition(Transform const & transform);

	Chunk & getChunkRelativeToPlayer();
	Vector3f getPlayerPosInChunkMap();
	Vector3f getPlayerPos();

	/**
	* pos = a position in the chunk map.
	*/
	Chunk & findChunk(Vector3f pos, int x, int z);
	void setPlayer(WeakPointer<Player> player);





	World();
	~World() = default;
};