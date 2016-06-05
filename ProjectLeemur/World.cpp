#include "World.h"
#include "Resources.h"
#include "Light.h"
#include "Keyboard.h"
#include "Skybox.h"

#include <chrono>

//void println(std::string const & v) {
//	std::cout << v << std::endl;
//}
Keyboard::Layout bindings;

double _parts = 9.8;
double _div = 1.5;
double _cutoff = 10.7;

void World::allowKeyBindings() {
	static bool once = false;
	if (once) return;
	once = true;

	// Testing values
	bindings.onKeyPressed(GLFW_KEY_P, [this](bool shifted) {
		changed = true;
		_parts += shifted ? -0.2 : 0.2;

		std::string out = "parts = ";
		out.append(std::to_string(_parts));
		out.append(", div = ");
		out.append(std::to_string(_div));
		out.append(", cutoff = ");
		out.append(std::to_string(_cutoff));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_O, [this](bool shifted) {
		changed = true;
		_div += shifted ? -0.2 : 0.2;

		std::string out = "parts = ";
		out.append(std::to_string(_parts));
		out.append(", div = ");
		out.append(std::to_string(_div));
		out.append(", cutoff = ");
		out.append(std::to_string(_cutoff));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_I, [this](bool shifted) {
		changed = true;
		_cutoff += shifted ? -0.2 : 0.2;

		std::string out = "parts = ";
		out.append(std::to_string(_parts));
		out.append(", div = ");
		out.append(std::to_string(_div));
		out.append(", cutoff = ");
		out.append(std::to_string(_cutoff));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_J, [this](bool shifted) {
		changed = true;
		terrain.persistence += shifted ? -0.25 : 0.25;

		std::string out = "persist = ";
		out.append(std::to_string(terrain.persistence));
		out.append(", freq = ");
		out.append(std::to_string(terrain.frequency));
		out.append(", amp = ");
		out.append(std::to_string(terrain.amplitude));
		out.append(", oct = ");
		out.append(std::to_string(terrain.octaves));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_K, [this](bool shifted) {
		changed = true;
		terrain.frequency *= shifted ? 0.5 : 2.0;

		std::string out = "persist = ";
		out.append(std::to_string(terrain.persistence));
		out.append(", freq = ");
		out.append(std::to_string(terrain.frequency));
		out.append(", amp = ");
		out.append(std::to_string(terrain.amplitude));
		out.append(", oct = ");
		out.append(std::to_string(terrain.octaves));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_L, [this](bool shifted) {
		changed = true;
		terrain.amplitude += shifted ? -0.1 : 0.1;

		std::string out = "persist = ";
		out.append(std::to_string(terrain.persistence));
		out.append(", freq = ");
		out.append(std::to_string(terrain.frequency));
		out.append(", amp = ");
		out.append(std::to_string(terrain.amplitude));
		out.append(", oct = ");
		out.append(std::to_string(terrain.octaves));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_H, [this](bool shifted) {
		changed = true;
		terrain.octaves += shifted ? -1 : 1;

		std::string out = "persist = ";
		out.append(std::to_string(terrain.persistence));
		out.append(", freq = ");
		out.append(std::to_string(terrain.frequency));
		out.append(", amp = ");
		out.append(std::to_string(terrain.amplitude));
		out.append(", oct = ");
		out.append(std::to_string(terrain.octaves));
		out.append("\n");
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_S, [this](bool shifted) {
		changed = true;
		terrain.seed = terrain.gen(0, 5000);
		std::string out = "seed: ";
		out.append(std::to_string(terrain.seed));
		printf(out.c_str());
	});

	bindings.onKeyPressed(GLFW_KEY_T, [this](bool shifted) {
		const static std::string key = toKey(0, 0);
		chunks[key]->printHeightMap();
	});

	Keyboard::addLayout(&bindings);
}

void World::onStart() {
	terrain = { // 0.5, 0.025, 10, 6, 100
		0.5, 0.0625, 1, 6, 3685	// 0.85freq
	};
	terrain.setOct1(1.0)
		//.setOct2(0.67)
		//.setOct3(0.22)
		.setElevationExp(1.71);

	allowKeyBindings();
	generateChunks();
	startChunks();
	evaluateChunks();
}

void World::onRender() {
	static Skybox & skybox = (Skybox &) Resources::getEntity(SKYBOX);

	Resources::getShader(TERRAIN_LIGHT).use();
	Light::Directional.loadToShaderi();
	Material::Grass.loadToShader();
	Material::Snow.loadToShader();
	Material::Sand.loadToShader();
	Material::Water.loadToShader();
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());

	renderChunks();
}

void World::onUpdate() {
	updateChunks();
	if (changed) {
		evaluateChunks();
		changed = false;
	}
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

void World::evaluateChunks() {
	for (auto & ref : chunks) {
		auto & chunk = ref.second;
		chunk->generateChunk(terrain);
	}
	for (auto & ref : chunks) {
		ref.second->renderMesh();
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
	Vector3f position(x << 4, 0, z << 4);
	addNewChunk(position, x, z);
}

void World::addNewChunk(Vector3f & chunkPos, int x, int z) {

	std::string key = toKey(x, z);
	if (containsKey(key)) return;

	chunks[key] = unique<Chunk>(this);
	Chunk & chunk = getChunk(key);
	chunk.transform.setPosition(chunkPos);
	chunk.setMapPosition(Vector3f(x, 0, z));
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

World::World() : terrain(0) {
}