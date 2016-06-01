#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Chunk.h"
#include "Transform.h"
#include "Player.h"

#ifdef _WIN32
#include <boost/unordered_map.hpp>
#endif

#define RENDER_DISTANCE 4

class World : public BaseEntity {
private:
	WeakPointer<Player> player;
	std::unordered_map<std::string, UniquePointer<Chunk>> chunks;

public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

	
	void generateChunks();
	void startChunks();
	void renderChunks();
	void updateChunks();
	void deleteChunks();
	
	void setNewChunk(int x, int z);
	void addNewChunk(int x, int z);
	void addNewChunk(Vector3f const & chunkPos, int x, int z);
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
	~World() = default;
};