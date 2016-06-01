#include "World.h"

//void println(std::string const & v) {
//	std::cout << v << std::endl;
//}

void World::onStart() {
	//generateChunks();
	startChunks();
}

void World::onRender() {
	renderChunks();
}

void World::onUpdate() {
	updateChunks();
}

void World::onDestroy() {
	deleteChunks();
}

void World::generateChunks() {
	const static int radius = RENDER_DISTANCE;

	int col = 1;
	int row = 1;
	int lay = 0;
	
	Vector3f position = getPlayerPos();
	std::string out = "Player position at: ";
	out.append(to_string(position));
	printf(out.c_str());

	addNewChunk(position.x, position.y);
	int x = position.x;
	int z = position.z;

	for (int i = 0; i < radius; i++) {
		int col_tmp = -col;
		int row_tmp = -row;

		// Proceed to build the matrix of chunks in a spiral

		// Top-left of matrix to right
		for (; col_tmp < col; col_tmp++) {
			addNewChunk(x + row_tmp, z + col_tmp);
		}
		// Top-right of matrix going down
		for (; row_tmp < row; row_tmp++) {
			addNewChunk(x + row_tmp, z + col_tmp);
		}

		// Bottom right of matrix going left
		for (; col_tmp != -col; col_tmp--) {
			addNewChunk(x + row_tmp, z + col_tmp);
		}

		// Bottom left of matrix going up
		for (; row_tmp != -row; row_tmp--) {
			addNewChunk(x + row_tmp, z + col_tmp);
		}

		col++;
		row++;
	}
}

void World::startChunks() {
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->onCreate();
	}
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->onStart();
	}
}

void World::renderChunks() {
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->onRender();
	}
}

void World::updateChunks() {
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->onUpdate();
	}
}

void World::deleteChunks() {
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->onDestroy();
		chunk.reset();
	}
	chunks.clear();
}

void World::setNewChunk(int x, int z) {
	std::string key = toKey(x, z);
	if (containsKey(key)) {
		chunks[key] = nullptr;
	}
	addNewChunk(x, z);
}

void World::addNewChunk(int x, int z) {
	Vector3f position(x << 3, 0, z << 3);
	addNewChunk(position, x, z);
}

void World::addNewChunk(Vector3f const & chunkPos, int x, int z) {
	std::string key = toKey(x, z);
	if (containsKey(key)) return;

	chunks[key] = unique<Chunk>(this);
	Chunk & chunk = getChunk(key);
	chunk.transform.setPosition(chunkPos);
	chunk.setPosition(Vector3f(x, 0, z));
}


std::string World::toKey(int x, int z) {
	std::string key = std::to_string(x);
	key.append("|");
	key.append(std::to_string(z));
	return key;
}

bool World::containsKey(int x, int z) {
	return containsKey(toKey(x, z));
}

bool World::containsKey(std::string const & key) {
	return chunks.find(key) != chunks.end();
}

Chunk & World::getChunk(int x, int z) {
	return getChunk(toKey(x, z));
}

Chunk & World::getChunk(std::string const & key) {
	if (!containsKey(key))
		return *Chunk::EMPTY;
	return *chunks[key];
}

Vector3f World::getChunkPosition(Transform const & transform) {
	Vector3f position = transform.getPosition();
	int x = (int)round(position.x / CHUNK_SIZE);
	int z = (int)round(position.z / CHUNK_SIZE);
	return { x, 0, z };
}

Chunk & World::getChunkRelativeToPlayer() {
	Vector3f pos = getPlayerPosInChunkMap();
	return *chunks[toKey(pos.x, pos.z)];
}

Vector3f World::getPlayerPosInChunkMap() {
	return getChunkPosition(player.lock()->transform);
}

Vector3f World::getPlayerPos() {
	return player.lock()->transform.getPosition();
}

Chunk & World::findChunk(Vector3f pos, int targetX, int targetZ) {
	int worldX = 0, worldZ = 0;

	while (targetX < 0 || targetX >= CHUNK_SIZE) {
		if (targetX < 0) {
			worldX--;
			targetX += CHUNK_SIZE;
			continue;
		}

		worldX++;
		targetX -= CHUNK_SIZE;
	}

	while (targetZ < 0 || targetZ >= CHUNK_SIZE) {
		if (targetZ < 0) {
			worldZ--;
			targetZ += CHUNK_SIZE;
			continue;
		}

		worldZ++;
		targetZ -= CHUNK_SIZE;
	}

	if (worldX != 0 || worldZ != 0) {
		worldX += pos.x;
		worldZ += pos.z;

		if (!containsKey(worldX, worldZ)) return *Chunk::EMPTY;

		return getChunk(worldX, worldZ);
	}

	return *Chunk::EMPTY;
}














void World::setPlayer(WeakPointer<Player> player) {
	this->player = player;
}

