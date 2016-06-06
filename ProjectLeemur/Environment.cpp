#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Building.h"
#include "Light.h"
#include "LSystem.h"

/* See the centrifuge for an example as to how to use
	the scene graph. NOTE: Animations are not implemented in
	the scene graph yet.
*/

SharedPointer<LSystem> test1, test2, test3;

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

#ifdef _WIN32
    test1 = std::make_shared<LSystem>("tree1.txt");
	test1->setSeed(3681);
	test2 = std::make_shared<LSystem>("tree2.txt");
	test2->setSeed(3688);
	test3 = std::make_shared<LSystem>("tree3.txt");
	test3->setSeed(3684);

#else
    test1 = std::make_shared<LSystem>("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/tree1.txt");
    test1->setSeed(3681);
    test2 = std::make_shared<LSystem>("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/tree2.txt");
    test2->setSeed(3688);
    test3 = std::make_shared<LSystem>("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/tree3.txt");
    test3->setSeed(3684);

#endif
	test1->turtle->setPosition(glm::vec3(8, 4, -5));
	test1->drawRules();
    test2->turtle->setPosition(glm::vec3(12, 8, -20));
    test2->drawRules();
	test3->turtle->setPosition(glm::vec3(20, 4, -10));
	test3->drawRules();
    test2->turtle->world->scaleLocal(1);
	
    addEntity((SharedPointer<Entity> &) test1->turtle);
    addEntity((SharedPointer<Entity> &) test2->turtle);
	addEntity((SharedPointer<Entity> &) test3->turtle);
	
	SharedPointer<Building> build = share<Building>(Vector3f(16, 6.4, -15), Vector3f(3,8,4), 1323);
	addEntity((SharedPointer<Entity> &) build);
	
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
//	delete (&Resources::getShader(TERRAIN_LIGHT));
	skybox->onDestroy();

	Mesh& pod = (Mesh&) Resources::getEntity(POD_OBJ);
	Mesh& cyl = (Mesh&) Resources::getEntity(CYL_OBJ);
	pod.destroy();
	cyl.destroy();

	delete &cyl;
	delete &pod;

	int size = entities.size();
	for (int i = 0; i < size; i++) {
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
