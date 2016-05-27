#pragma once

#include "Commons.h"
#include "Mob.h"
#include "Camera.h"

#include <queue>

class Player : public Mob {
private:
	UniquePointer<Camera> camera;
	UniquePointer<std::queue<Movement>> queue;

public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	Camera & getCamera() const;
};