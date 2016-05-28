#pragma once

#include "Commons.h"
#include "Mob.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Trackball.h"

#include <queue>

class Player : public Mob {
private:
	Keyboard::Layout keybind;
	Mouse::Layout mousebind;
	Trackball trackball;

	UniquePointer<Camera> camera;
	UniquePointer<std::queue<Movement>> queue;

public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	Camera & getCamera() const;



	Player();
};