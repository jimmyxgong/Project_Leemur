#include "Keyboard.h"
#include "Environment.h"
#include "Resources.h"

Keyboard::Layout Keyboard::defaultLayout;
Keyboard::Layout * Keyboard::layout;
std::stack<Keyboard::Layout*> Keyboard::layoutBackstack;

void Keyboard::init() {
	std::cout << "Keyboard initializng... " << std::endl;
	layoutBackstack.push(&defaultLayout);

	// Define the default layout here:
	defaultLayout.onKeyPressed(GLFW_KEY_E, [](bool isShifted) -> void {

	});

	defaultLayout.onKeyPressed(GLFW_KEY_I, [](bool) {

	});
}

void Keyboard::Layout::onKeyPressed(int key, std::function<void(bool)> l) {
	listeners.emplace(key, l);
}


void Keyboard::onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		bool isShifted = mods == GLFW_MOD_SHIFT;
		auto listener = topLayout().listeners.find(key);
		if (listener != topLayout().listeners.end()) {
			listener->second(isShifted);
		}
	}
}

void Keyboard::addLayout(Keyboard::Layout * layout) {
	layoutBackstack.push(layout);
}

Keyboard::Layout * Keyboard::popLayout() {
	// do not pop the default keyboard layout
	if (layoutBackstack.size() <= 1) return &defaultLayout;

	Layout * popped = layoutBackstack.top();
	layoutBackstack.pop();
	return popped;
}

Keyboard::Layout & Keyboard::topLayout() {
	return *layoutBackstack.top();
}


/*Deprecated*/
void Keyboard::setFocusedLayout(Layout * layout) {
	Keyboard::layout = layout;
}