#include "Player.h"
#include "Keyboard.h"
#include "Window.h"

void Player::onStart() {
	trackball = Trackball(&Window::getFocusedWindow());
	//transform.setPosition(Vector3f(1000, 1000, 1000));

	camera = unique<Camera>(Vector3f(0.0f, 6.0f, 2.2f));
	//camera->setLookAt(Vector3f(0.0f, 16.0f, 0.2f));
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
	//Keyboard::addLayout(&keybind);
	Window::getFocusedWindow().setActiveCamera(camera.get());
}

void Player::onRender() {

}

void Player::onUpdate() {
	//printf("Moving\n");
	while (!queue->empty()) {
		Movement m = queue->front(); 
		queue->pop();
		for (int i = 0; i < 4; i++)
			move(m);
	}

}

void Player::move(Movement move) {
	Vector3f m(0.f);
	if (move == Movement::FORWARD) {
		m.z = -0.5f;
	}
	else if (move == Movement::BACKWARD) {
		m.z = 0.5f;
	}
	else if (move == Movement::RIGHT) {
		m.x = 0.5f;
	}
	else m.x = -0.5f;

	camera->transform.translateLocal(m);
	camera->update();
}

Camera & Player::getCamera() const {
	return *camera.get();
}

Player::Player() {}