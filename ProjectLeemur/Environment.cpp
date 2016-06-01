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

	//world = unique<World>();

	centrifuge = unique<Centrifuge>();
	skybox = unique<Skybox>(window);
	player = share<Player>();
	Resources::addEntity(SKYBOX, skybox.get());
	Resources::addEntity(PLAYER, player.get());



	Window::getFocusedWindow().setActiveCamera(&player->getCamera());

	SharedPointer<World> world = unique<World>();
	world->setPlayer(player);
	//addEntity((SharedPointer<Entity> &) world);

	// Create pod and add a reference to what it should render.
	SharedPointer<GameObject> pod =
		share<GameObject>((Component*)&Resources::getEntity(POD_OBJ));
	addEntity((SharedPointer<Entity> &) pod);

	for (auto & entity : entities) {
		entity->onCreate();
	}
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
	for (auto & entity : entities) {
		entity->onStart();
	}

	player->onStart();
}

void Environment::onRender() {
	skybox->onRender();

	// call lighting:
	Light::Directional.loadToShader();

	
	for (auto & entity : entities) {
		entity->onRender();
	}
	player->onRender();
}

void Environment::onUpdate() {
	for (auto & entity : entities) {
		entity->onRender();
	}
	player->onUpdate();
}

void Environment::onDestroy() {
    delete (&Resources::getShader(SHADER_LIGHT));
    delete (&Resources::getShader(TOON_LIGHT));
	skybox->onDestroy();

	Mesh& pod = (Mesh&) Resources::getEntity(POD_OBJ);
	Mesh& cyl = (Mesh&) Resources::getEntity(CYL_OBJ);
	pod.destroy();
	cyl.destroy();

	delete &cyl;
	delete &pod;

	for (int i = 0; i < entities.size(); i++) {
		std::cout << "deleteing" << std::endl;
		entities[i]->onDestroy();
		entities[i] = nullptr;
	}
	entities.clear();
}

void Environment::addEntity(SharedPointer<Entity> & entity) {
	entities.push_back(std::move(entity));
}



UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}