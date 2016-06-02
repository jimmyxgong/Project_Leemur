#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Window.h"
#include "Player.h"
#include "Skybox.h"
#include "Chunk.h"

class Environment : public BaseEntity {
private:
	Window * window;
	UniquePointer<Skybox> skybox;
	SharedPointer<Player> player;


	/* Root of the game's scene graph */
	Transform transform;

	/* 
		Owned entities by the environment 
		Realisticially, the world transform root owns all
		entities, but for now we will have entities here 
		if we end up using it for some specifc reason.
	*/
	std::vector<SharedPointer<Entity>> entities;
	

public:
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;



	void addEntity(SharedPointer<Entity> & entity);
	static UniquePointer<Environment> create(Window * ref);
};