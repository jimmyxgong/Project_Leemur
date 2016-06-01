#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Light.h"
#include "Chunk.h"
#include "World.h"
#include "GameObject.h"

/* 
	See the centrifuge for an example as to how to use
	the scene graph. NOTE: Animations are not implemented in
	the scene graph yet.
*/
SharedPointer<Centrifuge> centrifuge;
UniquePointer<GameObject> pod;
//Chunk chunk;

// TODO LOGGER
void print(std::string const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Creating Environment...");

	centrifuge = unique<Centrifuge>();
	skybox = unique<Skybox>(window);
	player = share<Player>();
	Resources::addEntity(SKYBOX, skybox.get());
	Resources::addEntity(PLAYER, player.get());

	//GameObject obj = GameObject()
	pod = unique<GameObject>((Component*) &Resources::getEntity(POD_OBJ));
	//GameObject obj

	Window::getFocusedWindow().setActiveCamera(&player->getCamera());

	//UniquePointer<World> world = unique<World>();
	//world->setPlayer(player);

	// Skybox needs to be added first.
	//addEntity((UniquePointer<Entity> &) skybox);
	//addEntity((UniquePointer<Entity> &) world);
	//addEntity((UniquePointer<Entity> &) pod);

	for (auto & entity : entities) {
		entity->onCreate();
	}
	//pod->onCreate();
	centrifuge->onCreate();
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
	for (auto & entity : entities) {
		entity->onStart();
	}
	centrifuge->onStart();
	//pod->onStart();

	player->onStart();
	//world.addChild(player->transform);
}

void Environment::onRender() {
	skybox->onRender();
	for (auto & entity : entities) {
		entity->onRender();
	}
	player->onRender();
	//pod->onRender();
	centrifuge->onRender();
}

void Environment::onUpdate() {
	
	for (auto & entity : entities) {
		entity->onRender();
	}
	player->onUpdate();
	centrifuge->onUpdate();
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

	for (int i = 0; i < entities.size(); i++) {
		std::cout << "deleteing" << std::endl;
		entities[i]->onDestroy();
	}
	entities.clear();
}

void Environment::addEntity(UniquePointer<Entity> & entity) {
	entities.push_back(std::move(entity));
}



UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}