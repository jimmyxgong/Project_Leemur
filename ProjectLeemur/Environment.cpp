﻿#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Light.h"
#include "LSystem.h"

/* See the centrifuge for an example as to how to use
	the scene graph. NOTE: Animations are not implemented in
	the scene graph yet.
*/
SharedPointer<Centrifuge> centrifuge;
SharedPointer<LSystem> test1;

#include "Chunk.h"
#include "World.h"
#include "GameObject.h"

// TODO LOGGER
void print(std::string const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Creating Environment...");

	skybox = unique<Skybox>(window);
	player = share<Player>();
	Resources::addEntity(SKYBOX, skybox.get());
	Resources::addEntity(PLAYER, player.get());

	Window::getFocusedWindow().setActiveCamera(&player->getCamera());

	SharedPointer<World> world = share<World>();
	world->setPlayer(player);
	addEntity((SharedPointer<Entity> &) world);

	//test1 = std::make_shared<LSystem>("lsystemtest1.txt", 565);
	//test1->turtle->setPosition(glm::vec3(0, 0, 0));
	//test1->drawRules();
	//test1->turtle->world->updateAll();
	//test1->turtle->setIndices();
	//test1->turtle->initialize_mesh();
	
	//addEntity((SharedPointer<Entity> &) test1->turtle->obj);
	//addEntity((SharedPointer<Entity> &) test1->turtle);

	Window::getFocusedWindow()
		.setActiveCamera(&player->getCamera());

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
}

void Environment::onUpdate() {
	for (auto & entity : entities) {
		entity->onUpdate();
	}
	player->onUpdate();
}

void Environment::onDestroy() {
    delete (&Resources::getShader(SHADER_LIGHT));
    delete (&Resources::getShader(TOON_LIGHT));
	skybox->onDestroy();

	Mesh& pod = (Mesh&) Resources::getEntity(POD_OBJ);
	Mesh& cyl = (Mesh&) Resources::getEntity(CYL_OBJ);
	//Mesh& icos = (Mesh&)Resources::getEntity(ICOSAHEDRON_OBJ);
	pod.destroy();
	cyl.destroy();
	//icos.destroy();

	delete &cyl;
	delete &pod;
	//delete &icos;

	int size = entities.size();
	for (int i = 0; i < size; i++) {
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