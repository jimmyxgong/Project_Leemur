#include "Player.h"
#include "Keyboard.h"

Keyboard::Layout keybind;

void Player::onStart() {
	camera = unique<Camera>(transform.position);
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