#include "Keyboard.h"
#include "Environment.h"
#include "Resources.h"

Keyboard::Layout Keyboard::defaultLayout;
Keyboard::Layout * Keyboard::layout;

void Keyboard::init() {
	setFocusedLayout(&defaultLayout);
	map();
}

void Keyboard::map() {
	defaultLayout.onKeyPressed(GLFW_KEY_E, [](bool isShifted) -> void {

	});

	defaultLayout.onKeyPressed(GLFW_KEY_I, [](bool) {
	
	});
}

void Keyboard::Layout::onKeyPressed(int key, std::function<void(bool)> l) {
	listeners.emplace(key, l);
}


void Keyboard::onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		// Close the window. This causes the program to also terminate.
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		bool isShifted = mods == GLFW_MOD_SHIFT;
		auto listener = layout->listeners.find(key);
		if (listener != layout->listeners.end()) {
			listener->second(isShifted);
		}
	}
}

void Keyboard::setFocusedLayout(Layout * layout) {
	Keyboard::layout = layout;
}