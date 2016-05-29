#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Light.h"

UniquePointer<Centrifuge> centrifuge;

// TODO LOGGER
void print(const char * const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Creating Environment...");
	Light::init();

	skybox = unique<Skybox>(window);
	Resources::addEntity(SKYBOX, skybox.get());

	player = unique<Player>();
	Resources::addEntity(PLAYER, player.get());

	Resources::newObjEntity(POD_OBJ, "pod.obj");
	((Component&)Resources::getEntity(POD_OBJ))
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::RedPlastic);

	Resources::newObjEntity(CYL_OBJ, "cylinder.obj");
	((Component&)Resources::getEntity(CYL_OBJ))
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::Gold);

	centrifuge = unique<Centrifuge>();

	Window::getFocusedWindow()
		.setActiveCamera(&player->getCamera());
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
	player->onStart();
	centrifuge->onStart();

	// TODO add api for attaching entities
	//world.addChild(player->transform);
}

void Environment::onRender() {
	skybox->onRender();
	player->onRender();

	Light::Directional.loadToShader();
	centrifuge->onRender();
}

void Environment::onUpdate() {
	player->onUpdate();
	centrifuge->onUpdate();
}

void Environment::onDestroy() {
	delete (&Resources::getShader(SHADER_LIGHT));

	skybox->onDestroy();

	Component& pod = (Component&) Resources::getEntity(POD_OBJ);
	Component& cyl = (Component&) Resources::getEntity(CYL_OBJ);
	pod.onDestroy();
	cyl.onDestroy();

	delete &pod;
	delete &cyl;


}

UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}