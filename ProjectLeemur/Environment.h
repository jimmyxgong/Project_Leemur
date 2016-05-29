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

	/* Root of the game's scene graph */
	Transform world;

	/* 
		Owned entities by the environment 
		Realisticially, the world transform root owns all
		entities, but for now we will have entities here 
		if we end up using it for some specifc reason.
	*/
	std::vector<UniquePointer<Entity>> entities;
	

public:
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

	static UniquePointer<Environment> create(Window * ref);
};