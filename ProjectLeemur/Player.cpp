#include "Player.h"
#include "Keyboard.h"

Keyboard::Layout keybind;

void Player::onStart() {
	camera = unique<Camera>(transform.position);
	
	keybind.onKeyPressed(GLFW_KEY_W, [](bool isShifted) {
		std::cout << "Moving forward" << std::endl;

	});

}

void Player::onRender() {

}

void Player::onUpdate() {

}

Camera & Player::getCamera() const {
	return *camera.get();
}