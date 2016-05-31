#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Light.h"
#include "Chunk.h"

/* 
	See the centrifuge for an example as to how to use
	the scene graph. NOTE: Animations are not implemented in
	the scene graph yet.
*/
SharedPointer<Centrifuge> centrifuge;
//Chunk chunk;

// TODO LOGGER
void print(const char * const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Creating Environment...");

	//centrifuge = unique<Centrifuge>();
	skybox = unique<Skybox>(window);
	player = unique<Player>();
	Resources::addEntity(SKYBOX, skybox.get());
	Resources::addEntity(PLAYER, player.get());

	Window::getFocusedWindow().setActiveCamera(&player->getCamera());

	//chunk.onCreate();
	for (auto & entity : entities) {
		entity->onCreate();
	}
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
	player->onStart();

	for (auto & entity : entities) {
		entity->onStart();
	}


	//chunk.onStart();
	//centrifuge->onStart();
	//world.addChild(player->transform);
}

void Environment::onRender() {
	skybox->onRender();
	player->onRender();
	//chunk.onRender();


	//centrifuge->onRender();
}

void Environment::onUpdate() {
	player->onUpdate();
	//chunk.onUpdate();
	//centrifuge->onUpdate();
}

void Environment::onDestroy() {
	delete (&Resources::getShader(SHADER_LIGHT));

	skybox->onDestroy();

	Mesh& pod = (Mesh&) Resources::getEntity(POD_OBJ);
	Mesh& cyl = (Mesh&) Resources::getEntity(CYL_OBJ);
	pod.destroy();
	cyl.destroy();

	delete &pod;
	delete &cyl;
}


void Environment::buildWorld() {

}

void Environment::newChunk(int x, int y) {
	std::string key = std::to_string(x);
	key.append("|");
	key.append(std::to_string(y));
	chunks[key] = unique<Chunk>(this);
}



Environment& Environment::addEntity(UniquePointer<Entity> & entity) {
	entities.push_back(UniquePointer<Entity>(std::move(entity)));
	return *this;
}



UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}