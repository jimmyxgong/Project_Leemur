#include "Player.h"
#include "Keyboard.h"
#include "Window.h"

void Player::onStart() {
	trackball = Trackball(&Window::getFocusedWindow());

	camera = unique<Camera>(); // Vector3f(0.0f, 16.0f, 0.1f)
	queue = unique<std::queue<Movement>>();
	
	keybind.onKeyPressed(GLFW_KEY_W, [this](bool isShifted) {
		std::cout << "Moving forward" << std::endl;
		queue->push(Movement::FORWARD);
	});

	keybind.onKeyPressed(GLFW_KEY_A, [this](bool isShifted) {
		queue->push(Movement::LEFT);
	});
	
	keybind.onKeyPressed(GLFW_KEY_S, [this](bool isShifted) {
		queue->push(Movement::BACKWARD);
	});

	keybind.onKeyPressed(GLFW_KEY_D, [this](bool isShifted) {
		queue->push(Movement::RIGHT);
	});

	mousebind.setOnDrag([this](const Point & now, bool isLeft) {
		if (isLeft) {
			Quaternion rotation = trackball.fromPoints(Mouse::old, now);
			camera->transform.rotateLocal(rotation);
			camera->update();
		}
	});

	Mouse::pushLayout(&mousebind);
	Window::getFocusedWindow().setActiveCamera(camera.get());
}

void Player::onRender() {

}

void Player::onUpdate() {
	while (!queue->empty()) {
		Movement m = queue->front(); 
		queue->pop();
		move(m);
	}
}

Camera & Player::getCamera() const {
	return *camera.get();
}

Player::Player() {}