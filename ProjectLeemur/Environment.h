#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Window.h"
#include "Player.h"
#include "Skybox.h"

class Environment : public BaseEntity {
private:
	Window * window;
	UniquePointer<Skybox> skybox;
	UniquePointer<Player> player;

	/* Root of the game */
	Transform world;

public:
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

	static UniquePointer<Environment> create(Window * ref);
};