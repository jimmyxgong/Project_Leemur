#include "Environment.h"
#include "Skybox.h"
#include "Resources.h"
#include "Player.h"

UniquePointer<Skybox> skybox;
UniquePointer<Player> player;

// TODO LOGGER
void print(const char * const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Environment starting...");
	
	skybox = unique<Skybox>(window);
	Resources::addEntity(SKYBOX, skybox.get());

	player = unique<Player>();
	Resources::addEntity(PLAYER, player.get());


	Window::getFocusedWindow()
		.setActiveCamera(&player->getCamera());
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
}

void Environment::onRender() {
	skybox->onRender();
	player->onRender();
}

void Environment::onUpdate() {
	player->onUpdate();
}

void Environment::onDestroy() {
	skybox->onDestroy();
}

UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}