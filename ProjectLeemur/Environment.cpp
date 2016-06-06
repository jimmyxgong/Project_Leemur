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

SharedPointer<LSystem> test1, test2;

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

	//SharedPointer<World> world = share<World>();
	//world->setPlayer(player);
	//addEntity((SharedPointer<Entity> &) world);

#ifdef _WIN32
    test1 = std::make_shared<LSystem>("lsystemtest1.txt", 3685);
	test2 = std::make_shared<LSystem>("lsystemtest1.txt", 3683);

#else
    test1 = std::make_shared<LSystem>("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/lsystemtest1.txt", 3686);
    test2 = std::make_shared<LSystem>("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/lsystemtest1.txt", 3683);
#endif
	test1->turtle->setPosition(glm::vec3(5, 0, 0));
	test1->drawRules();
    test2->turtle->setPosition(glm::vec3(-5, 5, 0));
    test2->drawRules();
    test2->turtle->world->scaleLocal(.5);
	//test1->turtle->setIndices();
	//test1->turtle->initialize_mesh();
	
	//addEntity((SharedPointer<Entity> &) test1->turtle->obj);
    addEntity((SharedPointer<Entity> &) test1->turtle);
    addEntity((SharedPointer<Entity> &) test2->turtle);

	// Create pod and add a reference to what it should render.
	//SharedPointer<GameObject> pod =
	//	share<GameObject>((Component*)&Resources::getEntity(POD_OBJ));
	//addEntity((SharedPointer<Entity> &) pod);
	//addEntity((SharedPointer<Entity> &) share<Centrifuge>());
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
	//Mesh& icos = (Mesh&)Resources::getEntity(ICOSAHEDRON_OBJ);
	pod.destroy();
	cyl.destroy();
	//icos.destroy();

	delete &cyl;
	delete &pod;
	//delete &icos;

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
